#include "uart_console.h"

static void print_command_list() {
    char *arr_com[10];
    arr_com[0] = "\x1b[36mlog\x1b[0m           displaying temperature and humidity in console\n\r";
    arr_com[1] = "\x1b[36mled on\x1b[0m        [LED number(1 - 3)]\n\r";
    arr_com[2] = "\x1b[36mled off\x1b[0m       [LED number(1 - 3)]\n\r";
    arr_com[3] = "\x1b[36mled pulse\x1b[0m     [LED number(1 - 3)], [frequency(0.1 - 1.9)]\n\r";
    arr_com[4] = "\x1b[36mset_time\x1b[0m      [hh:mm:ss] set new time\n\r";
    arr_com[5] = "\x1b[36mset_alarm\x1b[0m     [hh:mm:ss] makes noise when that time comes\n\r";
    arr_com[6] = "\x1b[36mstop_alarm\x1b[0m    whithout parameters\n\r";
    arr_com[7] = "\x1b[36msound on\x1b[0m      make some noise\n\r";
    arr_com[8] = "\x1b[36msound off\x1b[0m     stop some noise\n\r";
    arr_com[9] = "\x1b[36mclear\x1b[0m         behavior standard command\n\r";

    for(int i = 0; i < 10; i++)
        uart_write_bytes(UART_NUM_1, arr_com[i], strlen(arr_com[i]));
}

int set_note (t_app *app, char **argv) {
    vTaskSuspend(app->acclr_task);
    if (argv[1]) {
        app->note = atoi(argv[1]);
        app->note *= -1;
        printf("app->note =     %d\n", app->note);// should be some function;
        memset(&app->note_to_oled, 0, 3);
        app->note = (app->pentatonic == 1) ? (pentatonic_mode(app->note, app->note_to_oled))
                                           : (chromatic_mode(app->note, app->note_to_oled));
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
//    vTaskResume(app->sound_task);
    xTaskCreate(&pwm_note_task, "pwm_note_task",
                2048u, app, 1, &app->sound_task);
}
void sound_off(t_app *app) {
    vTaskSuspend(app->sound_task);
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_25, GPIO_MODE_INPUT));
        //todo вимкнути pwm на gpio25 так само як і на ледах
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
            change_delay_cmd(argv);
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
        else if (!strcmp("standard_mode", argv[0])) {
            vTaskResume(app->acclr_task);
        }
        else if(!strcmp("wifi_sta_connect", argv[0])) {
            wifi_sta_connect((void *)cli->str_for_execute); // 1 аргумент - команда
        }
        else {
            uart_write_bytes( UART_NUM_1, ERR_COMM_NOT_FOUND, sizeof(ERR_COMM_NOT_FOUND));
            exit_status = EXIT_FAILURE;
        }
    }
    else {
        exit_status = EXIT_FAILURE;
    }
    free(argv);
    return exit_status;
}


