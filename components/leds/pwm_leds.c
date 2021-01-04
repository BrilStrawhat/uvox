#include "leds.h"

static void init_timer_chanel(ledc_timer_config_t *ledc_timer,
                       ledc_channel_config_t *ledc_channel, uint8_t led_num, uint8_t channel) {
    *ledc_timer = (ledc_timer_config_t){
            .duty_resolution = LEDC_TIMER_8_BIT,
            .freq_hz =  246,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num =  LEDC_TIMER_0,
            .clk_cfg = LEDC_AUTO_CLK,
    };
    *ledc_channel = (ledc_channel_config_t){
            .channel    = channel,
            .duty       = 1,
            .gpio_num   = led_num,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(ledc_channel);

}

static void pwm_leds(ledc_channel_config_t *ledc_channel, int16_t duty) {
    duty = -duty;
    duty = (duty < -250) ? -250 : duty;
    duty = (duty > 250) ? 250 : duty;
    duty += 252;
    duty = duty / 2;

    if (!ledc_channel)
        return;
    ledc_set_fade_with_time(ledc_channel->speed_mode,
                            ledc_channel->channel, duty, FADE_TIME);
    ledc_fade_start(ledc_channel->speed_mode,
                    ledc_channel->channel, LEDC_FADE_WAIT_DONE);
    vTaskDelay(30 / portTICK_PERIOD_MS);
}


void leds_on(void *arg) {
    t_app *app = (t_app *)arg;
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT));

    ledc_timer_config_t ledc_timer[LEDC_TEST_CH_NUM];
    ledc_channel_config_t ledc_channel;
    init_timer_chanel(&ledc_timer[0], &ledc_channel, LEDC_HS_CH0_GPIO, LEDC_HS_CH0_CHANNEL);
    init_timer_chanel(&ledc_timer[0], &ledc_channel, LEDC_HS_CH1_GPIO, LEDC_HS_CH0_CHANNEL);
    init_timer_chanel(&ledc_timer[0], &ledc_channel, LEDC_HS_CH2_GPIO, LEDC_HS_CH0_CHANNEL);
    ledc_timer_config(ledc_timer);
    ledc_fade_func_install(0);

    while(1) {
        pwm_leds(&ledc_channel, app->acclr[0]);
        vTaskDelay(LEDS_DELAY / portTICK_PERIOD_MS);
    }
}

int8_t leds_off(t_app *app, char **argv) {
    if (!argv[1]) {
        if(app->leds_task != NULL) {
            vTaskDelete(app->leds_task);
            app->leds_task = NULL;
        }
        ledc_fade_func_uninstall();
        ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_INPUT));
        ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_INPUT));
        ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_INPUT));
        return 0;
    }
    else {
        uart_write_bytes(UART_NUM_1, ERR_TO_MANY_ARG, sizeof(ERR_TO_MANY_ARG));
        return -1;
    }
}

