#include "accelerometer.h"
#include "main.h"
#include "buttons.h"

void app_main(void) {
    spi_device_handle_t spi;
    
    accel_config(&spi);
    buttons_init();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    uart_config_t uart_config = {
        .baud_rate  = 9600,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    
    add_cmd(WIFI_STA_CONNECT_COMMAND, wifi_sta_connect);
    init_uart_console(&uart_config, 1);
    wifi_connect_nvs();

    xTaskCreate(read_acceleration_task, "read_acceleration_task",
                2048u, (void*)spi, 2, 0);
}
