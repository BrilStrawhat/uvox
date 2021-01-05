#include "sound.h"

static const char *TAG = "SOUND: ";

static void init(ledc_timer_config_t *ledc_timer, ledc_channel_config_t *ledc_channel) {
    *ledc_timer = (ledc_timer_config_t){
            .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
            .freq_hz = B_LINE_1,                   // frequency of PWM signal
            .speed_mode = LEDC_LS_MODE,           // timer mode
            .timer_num = LEDC_LS_TIMER,            // timer index
            .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    *ledc_channel = (ledc_channel_config_t){
            .channel    = LEDC_LS_CH1_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH1_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
    };
    ledc_channel_config(ledc_channel);
    ledc_timer_config(ledc_timer);
}

void pwm_note_task(void *arg) {
    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel;
    init(&ledc_timer, &ledc_channel);
    t_display *display = malloc(sizeof(t_display));
    oled_init(display);
    t_app *app = (t_app *) arg;
    uint16_t old_note = 0;
    int16_t duty = 0;
    uint8_t io_num;

    esp_log_level_set(TAG, ESP_LOG_NONE);
    while (1) {
            duty = app->acclr[1];
            if (duty < 0)
                duty *= -2;
            else if (duty == 0)
                duty = 16;
            else
                duty *= 16;
            ESP_LOGI(TAG,"DUTY = %d", duty);
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
            if (old_note != app->note) {
                ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, app->note);
                oled_clear(display);
                send_to_oled(display, app->note_to_oled, NULL); // NULL - for duty
                old_note = app->note;
            }
        while (xQueueReceive(gpio_button_evt_queue, &io_num, 0)) {
            if (io_num == GPIO_INPUT_IO_0 && app->delay < 60000) {
                app->delay += 50;
            }
            else if (app->delay > 51) {
                app->delay -= 50;
            }
        }
        vTaskDelay(app->delay / portTICK_PERIOD_MS);
    }

}
