#include "accelerometer.h"

void app_main(void) {
    spi_device_handle_t spi;
    accel_config(&spi);
    config_leds_and_beeep();
    xTaskCreate(read_acceleration_task, "read_acceleration_task",
                2048u, (void*)spi, 2, 0);
//    xTaskCreate(notes, "read_acceleration_task",
//                2048u, (void*)spi, 2, 0);
    notes();
}
