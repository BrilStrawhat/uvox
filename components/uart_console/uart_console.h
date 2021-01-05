#ifndef UART_CONSOLE
#define UART_CONSOLE

#define UART_TX_PIN                     17
#define UART_RX_PIN                     16


#define SIZE_STR_FOR_EXECUTE            120
#define ENTER                           13
#define MAX_SIZE_BUF                    120
#define DELETE                          127
#define BUF_SIZE                        1024

#include "mx_function.h"
#include "components.h"
#include "accelerometer.h"
#include "leds.h"
#include "sound.h"
#include "buttons.h"
#include "wifi.h"


#define NL_SEQ      "\eE"

typedef struct s_cli {
    uint8_t         *buf;
    char            *str_for_execute;
    int             iterator;
    int             len;
} t_cli;

void data_from_uart(void *data);
int command_execution(t_cli *cli,  t_app *app);
void simple_simbols_handler(t_cli *cli, t_app *app);
_Bool printable_char(char *buf);

#endif
