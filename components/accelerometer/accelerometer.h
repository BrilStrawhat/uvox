#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#define REG_POWER_CTL       0x2Du
#define POWER_CTL_MEASURE   0x08u

#define PIN_NUM_MISO    12
#define PIN_NUM_MOSI    13
#define PIN_NUM_CLK     14
#define PIN_NUM_CS      15

#define GPIO_ACCEL          23
#define ADXL_HOST           VSPI_HOST
#define DMA_CHAN            0

#define REG_DEVID           0x00u
#define REG_BW_RATE         0x2Cu

#define REG_READ_FLAG       0x80u
#define REG_MB_FLAG         0x40u
#define REG_DATAX0          0x32u
#define UPDATE_DELAY        (200u / portTICK_PERIOD_MS)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include <driver/dac.h>

void accel_config(spi_device_handle_t *spi);
void read_acceleration_task(void* pvParameters);
void leds(int value);
void beeep(void);
void config_leds_and_beeep();
void notes(int16_t freq, int16_t  duty);
#endif
