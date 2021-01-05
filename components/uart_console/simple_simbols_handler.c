#include "uart_console.h"

static void handling_ENTER(t_cli *app) {
    uart_write_bytes( UART_NUM_1, "\n\r", sizeof( "\n" ));
    app->iterator = 0;
}

static void handling_DELETE(t_cli *cli) {
    if (cli->iterator == strlen(cli->str_for_execute ) && cli->iterator > 0) {
        uart_write_bytes( UART_NUM_1, "\e[1D", sizeof( "\e[1D" ));
        uart_write_bytes( UART_NUM_1, " ", 1 );
        uart_write_bytes( UART_NUM_1, "\e[1D", sizeof( "\e[1D" ));
        cli->str_for_execute[strlen(cli->str_for_execute) - 1] = '\0';
        (cli->iterator > 0) ? cli->iterator-- : 0;
    }
}

static void handling_ARROWS(t_cli *cli) {
    if (cli->buf[0] == 27 && cli->buf[1] == 91) {
        if (cli->buf[2] == 67) {

            if (cli->iterator < strlen(cli->str_for_execute)) {
                uart_write_bytes(UART_NUM_1, &cli->str_for_execute[cli->iterator],
                                 sizeof(cli->str_for_execute[cli->iterator]));
                cli->iterator++;
            }
        }
        if (cli->buf[2] == 68) {
            if (cli->iterator <= 0)
                cli->iterator = 0;
            else {
                cli->iterator--;
                uart_write_bytes(UART_NUM_1, "\b", 1);
            }
        }
    }
}

void simple_simbols_handler(t_cli *cli, t_app *app) {
    if (cli->len == 1) {
        if (cli->buf[0] == DELETE) {
            handling_DELETE(cli);
        }
        else if (cli->buf[0] == ENTER) {
            handling_ENTER(cli);
            command_execution(cli, app);
            memset(cli->str_for_execute, 0, SIZE_STR_FOR_EXECUTE );
        }
        else if (printable_char((char *) cli->buf)) {
            cli->iterator = cli->iterator + 1;
            uart_write_bytes(UART_NUM_1, (char *) cli->buf, sizeof(cli->buf));
        }
    }
    else if (cli->len == 3 && cli->buf[0] == 27 && cli->buf[1] == 91) {
        handling_ARROWS(cli);
    }
    else {
        uart_write_bytes(UART_NUM_1, &cli->str_for_execute[cli->iterator],
                         strlen(cli->str_for_execute) - cli->iterator);
        uart_write_bytes(UART_NUM_1, &cli->str_for_execute[cli->iterator],
                         strlen(cli->str_for_execute) - cli->iterator);
        cli->iterator = strlen(cli->str_for_execute);
    }
}

