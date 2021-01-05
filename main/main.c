#include "accelerometer.h"
#include "main.h"
#include "buttons.h"
#include "wifi.h"
#include "uart_console.h"

void init_struct(t_app *app) {
    app->leds_task = NULL;
    app->oled_task = NULL;
    app->sound_task = NULL;
    app->acclr_task = NULL;
    app->note = 0;
    memset(&app->note_to_oled, 0, 3);
    app->pentatonic = 1;
    app->delay = 500;
    app->duty = 1;
}

void app_main(void) {
    buttons_init();

    t_app *app = malloc(sizeof(t_app));
    if(!app) return;
    init_struct(app);
    xTaskCreate(&data_from_uart, "data_from_uart", 4096u,
            app, 4, NULL);
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_connect_nvs();
    xTaskCreate(&read_acceleration_task, "read_acceleration_task",
            2048u, app, 1, &app->acclr_task);
    xTaskCreate(&pwm_note_task, "pwm_note_task",
            2048u, app, 1, &app->sound_task);
    xTaskCreate(leds_on, "leds_on", 2048, app, 4, &app->leds_task);
}
