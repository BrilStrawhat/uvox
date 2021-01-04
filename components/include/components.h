#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/timer.h"

#include "esp_err.h"
#include "driver/i2c.h"
#include "driver/ledc.h"
#include "driver/uart.h"

///////////////////////////////////////////////////
#define ERR_COMM_NOT_FOUND              "\n\x1b[31mrtos: command not found\n\r\x1b[0m"
#define ERR_TO_MANY_ARG                 "\n\x1b[31mrtos: too many arguments\n\r\x1b[0m"
#define ERR_BAD_ARG                     "\n\x1b[31mrtos: bad argument\n\r\x1b[0m"
#define ERR_ADD_PARAM                   "\n\x1b[31mrtos: please add parameters\n\r\x1b[0m"


////////////////////////////////////////////////////        notes
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
////////////////////////////////////////////////////

#define DEFAULT_DUTY            0x0


typedef struct s_display {
    uint8_t             addr;           // I2C address
    i2c_port_t          port;           // I2C interface port
    uint16_t            changes;        // page change bit to optimize writes
    uint8_t *           font_str;       // page buffer
} t_display;


typedef struct s_app {
    TaskHandle_t	leds_task;
    TaskHandle_t	oled_task;
    TaskHandle_t	sound_task;
    TaskHandle_t	acclr_task;
    int16_t         acclr[3];
    uint16_t        note;
    bool            pentatonic;
    char            note_to_oled[3];
} t_app;

void uart_print_nl();
void uart_printstr(char *str);

#endif
