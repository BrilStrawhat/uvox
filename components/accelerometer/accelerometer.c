#include "accelerometer.h"
#include "buttons.h"

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




void read_acceleration_task(void* pvParameters) {
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT));
    t_display *display = malloc(sizeof(t_display));
    oled_init(display);
    int16_t accs[3];
    spi_device_handle_t spi = (spi_device_handle_t) pvParameters;
    char *note_to_oled = malloc(2);
    uint16_t note = 0;
    uint16_t old_note = 0;
    uint16_t delay = 500;
    uint8_t io_num;


    while (1) {
        memset(note_to_oled, 0, 2);
        read_acceleration(spi, accs);
        printf("xyz %d      %d      %d\n", (int) accs[0], (int) accs[1], (int) accs[2]);
//        note = chromatic_mode((int)accs[0], &note_to_oled);
        pwm_leds(((int) accs[0]));
        oled_clear(display);
        send_to_oled(display, note_to_oled, NULL); // NULL - for duty

        if (xQueueReceive(gpio_button_evt_queue, &io_num, 0)) {
            if (io_num == GPIO_INPUT_IO_0)
                note = pentatonic_mode((accs[0]), &note_to_oled);
            if (old_note != note) {
                notes(note, accs[1]);
                old_note = note;
            }
            while (xQueueReceive(gpio_button_evt_queue, &io_num, 0)) {
                if (io_num == GPIO_INPUT_IO_0 && delay < 65500)
                    delay += 50;
                else if (delay > 0)
                    delay -= 50;
            }
            vTaskDelay(delay / portTICK_PERIOD_MS);
        }
    }
}


