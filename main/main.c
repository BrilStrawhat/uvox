#include "accelerometer.h"
#include "main.h"
#include "buttons.h"

void app_main(void) {
    spi_device_handle_t spi;
    
    accel_config(&spi);
    buttons_init();

    xTaskCreate(read_acceleration_task, "read_acceleration_task",
                2048u, (void*)spi, 2, 0);
    // xTaskCreate(read_acceleration_task, "read_acceleration_task",
                // 2048u, (void*)spi, 2, &accelerometer_task_handler);
}
