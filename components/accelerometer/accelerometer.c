#include "accelerometer.h"
#include "buttons.h"

#define B_SMALL         123
#define C_LINE_1        131
#define CIS_LINE_1      138
#define D_LINE_1        147
#define DIS_LINE_1      155
#define E_LINE_1        165
#define EIS_LINE_1      175
#define FIS_LINE_1      185
#define G_LINE_1        196
#define GIS_LINE_1      208
#define A_LINE_1        220
#define AIS_LINE_1      233
#define B_LINE_1        246
#define C_LINE_2        262
#define CIS_LINE_2      277
#define D_LINE_2        294
#define DIS_LINE_2      311
#define E_LINE_2        330
#define EIS_LINE_2      349
#define FIS_LINE_2      370
#define G_LINE_2        392
#define GIS_LINE_2      415
#define A_LINE_2        440
#define AIS_LINE_2      466
#define B_LINE_3        494



static void print_error(char *str) {
    write(2, str, strlen(str));
}

static void power_managment(spi_device_handle_t spi, uint8_t reg_power_ctl,
                            uint8_t power_ctl_measure) {
    spi_transaction_t trans = {
            .cmd = reg_power_ctl,
            .tx_buffer = &power_ctl_measure,
            .length = 8 // in bits
    };
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi, &trans));
}

void accel_config(spi_device_handle_t *spi) {
    if(!spi) {
        print_error("ERROR: structure of type spi_device_handle_t *handle is expected\n");
        exit(1);
    }
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 1));

    spi_bus_config_t bus = {
            .miso_io_num = PIN_NUM_MISO,
            .mosi_io_num = PIN_NUM_MOSI,
            .sclk_io_num = PIN_NUM_CLK,
            .quadwp_io_num = -1, //maybe to clear those 2 strings
            .quadhd_io_num = -1,
    };
    spi_device_interface_config_t device  = {
            .clock_speed_hz = 1000000,
            .mode = 3,
            .spics_io_num = PIN_NUM_CS,
            .command_bits = 8,       // 1 byte in 1 transaction
            .queue_size = 1       // 1 bit in queue
        };

    ESP_ERROR_CHECK(spi_bus_initialize(ADXL_HOST, &bus, DMA_CHAN));
    ESP_ERROR_CHECK(spi_bus_add_device(ADXL_HOST, &device, spi));
    power_managment(*spi, REG_POWER_CTL, POWER_CTL_MEASURE);
}

static void read_acceleration (spi_device_handle_t spi, int16_t *accs) {
    uint8_t tx_buffer[3U * sizeof(uint16_t)];

    spi_transaction_t trans = {
            .cmd = REG_READ_FLAG | REG_MB_FLAG | REG_DATAX0, //0x80U | 0x40U | 0x32U, //
            .length = sizeof(tx_buffer) * 8, // in bits (not in byte)
            .tx_buffer = tx_buffer,
            .rx_buffer = accs
    };
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi, &trans));
}

int small(int accel_data) {
    if(accel_data >= -42 && accel_data < 42) {
        printf("b\n");
        return B_LINE_1;
    }
    else if (accel_data >= 42 && accel_data < 84) {
        printf("cis\n");
        return A_LINE_1;
    }
    else if (accel_data >= 84  && accel_data < 126) {
        printf("d\n");
        return FIS_LINE_1;
    }
    else if (accel_data >= 126  && accel_data < 168) {
        printf("fis\n");
        return D_LINE_1;
    }
    else if (accel_data >= 168  && accel_data < 210) {
        printf("a\n");
        return CIS_LINE_1;
    }
    else if (accel_data >= 210) {
        printf("bb\n");
        return B_SMALL;
    }
    else
        return 0;
}

int line_1(int accel_data) {
    if (accel_data <= -42 && accel_data > -84) {
        printf("a_m\n");
        return CIS_LINE_2;
    }
    else if (accel_data <= -84 && accel_data > -126) {
        printf("fis_m\n");
        return D_LINE_2;
    }
    else if (accel_data <= -126 && accel_data > -168) {
        printf("d_m\n");
        return FIS_LINE_2;
    }
    else if (accel_data <= -168 && accel_data > -210) {
        printf("cis_m\n");
        return A_LINE_2;
    }
    else if (accel_data <= -210) {
        printf("b_m\n");
        return B_LINE_3;
    }
    else {
        printf("bad arg\n");
        return 0;
    }
}

int pentatonic_mode(int accel_data) {
    int note = 0;

    if ((note = line_1(accel_data)) != 0)
        return note;
    note = small(accel_data);
    return note;
}

// void chromatic_mode(int accel_data, int *note) {

// }





void read_acceleration_task(void* pvParameters) {
    int16_t accs[3];
    spi_device_handle_t spi = (spi_device_handle_t)pvParameters;
    int note = 0;
    uint16_t delay = 1000;
    uint32_t io_num;

    while (1) {
        read_acceleration(spi, accs);
        // printf("xyz %d      %d      %d\n", (int)accs[0], (int)accs[1], (int)accs[2]);
        note = pentatonic_mode((int)accs[0]);
        // printf("note = %d accs[1] = %d\n", note, accs[1]);
        notes(note, accs[1]);
        if(xQueueReceive(gpio_button_evt_queue, &io_num, 0)) {
            if (io_num == GPIO_INPUT_IO_0)
                delay += 50;
            else
                delay -= 50;
        }
        printf("%d\n", delay);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
}

