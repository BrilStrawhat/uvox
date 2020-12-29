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
#include "driver/uart.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/timer.h"

#include "esp_err.h"
#include "driver/ledc.h"
#include "driver/i2c.h"
#include "driver/i2c.h"

////////////////////////////////////////////////////      console
#define UART_TX_PIN                     17
#define UART_RX_PIN                     16
#define BUF_SIZE                        1024

#define ERR_COMM_NOT_FOUND              "\x1b[31mrtos: command not found\n\r\x1b[0m"
#define ERR_TO_MANY_ARG                 "\x1b[31mrtos: too many arguments\n\r\x1b[0m"
#define ERR_BAD_ARG                     "\x1b[31mrtos: bad argument\n\r\x1b[0m"
#define ERR_ADD_PARAM                   "\x1b[31mrtos: please add parameters\n\r\x1b[0m"

#define SIZE_STR_FOR_EXECUTE            120
#define ENTER                           13
#define MAX_SIZE_BUF                    120
#define DELETE                          127
////////////////////////////////////////////////////

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
    uint8_t         *buf;
    char            *str_for_execute;
    int             iterator;
    int             len;
    int             history_iterator;
} t_app;

#endif
