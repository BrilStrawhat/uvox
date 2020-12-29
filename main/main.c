#include "accelerometer.h"

void app_main(void) {
    spi_device_handle_t spi;
    accel_config(&spi);

    xTaskCreate(read_acceleration_task, "read_acceleration_task",
                2048u, (void*)spi, 2, 0);
}
