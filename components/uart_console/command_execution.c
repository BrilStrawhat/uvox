#include "uart_console.h"

static void print_command_list() {
    char *arr_com[10];
    arr_com[0] = "\n\x1b[36mleds_on\x1b[0m\n\r";
    arr_com[1] = "\x1b[36mleds_off\x1b[0m\n\r";
    arr_com[2] = "\x1b[36mset_note\x1b[0m\n\r";
    arr_com[3] = "\x1b[36mset_temp\x1b[0m\n\r";
    arr_com[4] = "\x1b[36mset_scale\x1b[0m\n\r";
    arr_com[5] = "\x1b[36msound_off\x1b[0m\n\r";
    arr_com[6] = "\x1b[36msound_on\x1b[0m\n\r";
    arr_com[7] = "\x1b[36macclr\x1b[0m\n\r";
    arr_com[8] = "\x1b[36mwifi_sta_connect\x1b[0m\n\r";
    arr_com[9] = "\x1b[36mclear\x1b[0m\n\r";

    for(int i = 0; i < 10; i++)
        uart_write_bytes(UART_NUM_1, arr_com[i], strlen(arr_com[i]));
}

int set_note (t_app *app, char **argv) {
    vTaskSuspend(app->acclr_task);
    if (argv[1]) {
        app->acclr[0] = atoi(argv[1]);
        app->acclr[0] *= -1;
        printf("app->note =     %d\n", app->acclr[0]);// should be some function;
        memset(&app->note_to_oled, 0, 3);
        app->note = (app->pentatonic == 1) ? (pentatonic_mode(app->acclr[0], app->note_to_oled))
                                           : (chromatic_mode(app->acclr[0], app->note_to_oled));
        printf("NOTE = %s\n", app->note_to_oled);
        return 0;
    }
    else {
        uart_write_bytes(UART_NUM_1, ERR_ADD_PARAM, sizeof(ERR_ADD_PARAM));
        return -1;
    }

}

int set_scale(t_app *app, char **argv) {
    if(argv[1]) {
        if (!strcmp("chromatic", argv[1])) {
            app->pentatonic = 0;
            return 0;
        }
        else if (!strcmp("pentatonic", argv[1])) {
            app->pentatonic = 1;
            return 0;
        }
        else {
            app->pentatonic = 1;
            uart_write_bytes(UART_NUM_1, ERR_BAD_ARG, sizeof(ERR_BAD_ARG));
            return -1;
        }
    }
    else {
        uart_write_bytes(UART_NUM_1, ERR_ADD_PARAM, sizeof(ERR_ADD_PARAM));
        return -1;
    }
}

void sound_on(t_app *app) {
    if(app->oled_task) {
        vTaskSuspend(app->oled_task);
    }
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_25, GPIO_MODE_OUTPUT));
    vTaskResume(app->sound_task);
}
void sound_off(t_app *app) {
    vTaskSuspend(app->sound_task);

    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_25, GPIO_MODE_INPUT));
    if(!app->oled_task) {
        xTaskCreate(&data_to_oled, "data_to_oled", 2048u, app, 3, &app->oled_task);
    }
    else {
        vTaskResume(app->oled_task);
    }
}


int command_execution(t_cli *cli,  t_app *app) {
    int8_t exit_status = -1;
    char **argv = mx_strsplit(cli->str_for_execute, ' ');
    int argc = mx_arr_size(argv);

        if (argc) {
        if(!strcmp("help", argv[0])) {
            print_command_list();
        }
        else if(!strcmp("leds_on", argv[0])) {
            xTaskCreate(leds_on, "leds_on", 2048, app, 3, &app->leds_task);
        }
        else if(!strcmp("leds_off", argv[0])) {
            exit_status = leds_off(app, argv);
        }
        else if (!strcmp("set_note", argv[0])) { //freq duty
            exit_status = set_note(app, argv);
        }
        else if(!strcmp("set_temp", argv[0])) {
            exit_status = change_delay_cmd(app, argv);
        }
        else if (!strcmp("set_scale", argv[0])) {
            exit_status = set_scale(app, argv);
        }
        else if (!strcmp("sound_off", argv[0])) {
            sound_off(app);
        }
        else if (!strcmp("sound_on", argv[0])) {
           sound_on(app);
        }
        else if (!strcmp("d", argv[0]) && argv[1]) {
            app->duty = atoi(argv[1]);
        }
        else if (!strcmp("acclr_mode", argv[0])) {
            vTaskResume(app->acclr_task);
        }
        else if(!strcmp(WIFI_STA_CONNECT_COMMAND , argv[0])) {
            wifi_sta_connect((void *)cli->str_for_execute); // 1 аргумент - команда
        }
        else if(!strcmp("clear", argv[0]) && !argv[1]) {
            uart_write_bytes(UART_NUM_1, "\e[2J",sizeof("\e[2J"));
            uart_write_bytes(UART_NUM_1, "\e[0;0f",sizeof("\e[0;0f"));
        }
        else {
            uart_write_bytes( UART_NUM_1, ERR_COMM_NOT_FOUND, sizeof(ERR_COMM_NOT_FOUND));
            exit_status = EXIT_FAILURE;
        }
        mx_free_array(argv, argc);
    }
    else {
        exit_status = EXIT_FAILURE;
    }

    return exit_status;
}


