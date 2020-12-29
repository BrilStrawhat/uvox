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




//////////////////
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

///////////////////////////////





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
#include "driver/ledc.h"


void accel_config(spi_device_handle_t *spi);
void read_acceleration_task(void* pvParameters);
void notes(void);

int pentatonic_mode(int accel_data);
int chromatic_mode(int accel_data);

void pwm_notes(int nota);
#endif