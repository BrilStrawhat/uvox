#include "uart_console.h"

static QueueHandle_t uart_queue;

void uart_printstr(char *str) {
    if (str)
        uart_write_bytes(UART_NUM_1, str, strlen(str));
}

void uart_print_nl() {
    uart_write_bytes(UART_NUM_1, NL_SEQ, 2);
}

void init_console_data(t_cli *cli) {
    cli->buf = malloc(BUF_SIZE);
    cli->str_for_execute = malloc(SIZE_STR_FOR_EXECUTE);
    memset(cli->str_for_execute, 0, SIZE_STR_FOR_EXECUTE);
    cli->iterator = 0;
    cli->len = 0;
}

void uart_config(void) {
    uart_config_t uart_config =
            {
                    .baud_rate = 9600,
                    .data_bits = UART_DATA_8_BITS,
                    .parity = UART_PARITY_DISABLE,
                    .stop_bits = UART_STOP_BITS_2,
                    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
            };
    ESP_ERROR_CHECK( uart_param_config( UART_NUM_1, &uart_config ));
    ESP_ERROR_CHECK( uart_set_pin( UART_NUM_1, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE ));
    ESP_ERROR_CHECK( uart_driver_install( UART_NUM_1, 1024, 1024, 10, &uart_queue, 0 ));
    uart_pattern_queue_reset( UART_NUM_1, 20 );
}


_Bool printable_char(char *buf) {
    for (int i = 0; i < strlen( buf ); i++) {
        if (buf[i] < 32 || buf[i] > 126)
            return 0;
        i++;
    }
    return 1;
}

_Bool is_buffer_enought(t_cli *cli) {
    if ((cli->len == 1 && (cli->buf[0] == ENTER || cli->buf[0] == DELETE))
        || (cli->len == 3 && (cli->buf[0] == 27 && cli->buf[1] == 91))) {
        return 1;
    }
    if (strlen((char *) cli->buf) + strlen(cli->str_for_execute) < SIZE_STR_FOR_EXECUTE) {
        if (printable_char((char *) cli->buf )) {
            strcat(cli->str_for_execute, (char *) cli->buf);
        }
        return 1;
    }
    return 0;
}

void data_from_uart(void *data) {
    uart_config();
    uart_event_t event;
    t_app *app = (t_app*)data;
    t_cli *cli = malloc(sizeof(t_cli));
    init_console_data(cli);


    while (1) {
        if (xQueueReceive(uart_queue, (void *) &event,
                          (portTickType) portMAX_DELAY)) {
            if (event.size < MAX_SIZE_BUF && event.type == UART_DATA) {
                memset(cli->buf,0, BUF_SIZE);
                cli->len = uart_read_bytes(UART_NUM_1, cli->buf, event.size, portMAX_DELAY);
                if (is_buffer_enought(cli)) {
                    simple_simbols_handler(cli, app);
                }
            }
            uart_flush_input( UART_NUM_1 );
        }
        vTaskDelay( 10 / portTICK_PERIOD_MS );
    }
}
