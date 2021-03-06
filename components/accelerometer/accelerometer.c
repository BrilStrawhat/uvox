#include "accelerometer.h"

static const char *TAG = "ACCLR: ";


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




void read_acceleration_task(void* arg) {
    spi_device_handle_t spi;
    accel_config(&spi);
    t_app *app = (t_app *)arg;

    esp_log_level_set(TAG, ESP_LOG_NONE);
    while (1) {
        read_acceleration(spi, app->acclr);
        ESP_LOGI(TAG, "xyz %d      %d      %d",
                (int) app->acclr[0], (int) app->acclr[1], (int) app->acclr[2]);

        app->note = (app->pentatonic == 1) ? (pentatonic_mode(app->acclr[0], app->note_to_oled))
                : (chromatic_mode(app->acclr[0], app->note_to_oled));
        vTaskDelay(10/ portTICK_PERIOD_MS);
    }
}
