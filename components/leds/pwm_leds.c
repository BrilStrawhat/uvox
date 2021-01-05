#include "leds.h"

static void init_timer_chanel(ledc_channel_config_t *ledc_channel,
                              uint8_t led_num) {
    *ledc_channel = (ledc_channel_config_t){
            .channel    = LEDC_HS_CH0_CHANNEL,
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

    ledc_channel_config_t ledc_channel;
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz =  246,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num =  LEDC_TIMER_0,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&ledc_timer);
    init_timer_chanel(&ledc_channel, LEDC_HS_CH0_GPIO);
    init_timer_chanel(&ledc_channel, LEDC_HS_CH1_GPIO);
    init_timer_chanel(&ledc_channel, LEDC_HS_CH2_GPIO);
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
        ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_HS_CH0_CHANNEL, 0);
        ledc_timer_rst(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0);
        ledc_fade_func_uninstall();
        return 0;
    }
    else {
        uart_write_bytes(UART_NUM_1, ERR_TO_MANY_ARG, sizeof(ERR_TO_MANY_ARG));
        return -1;
    }
}

