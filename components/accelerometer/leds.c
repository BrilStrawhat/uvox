#include "accelerometer.h"

#define GPIO_LED_1 26
#define MAX_DUTY 255
#define MIN_FADE 0
#define FADE_TIME 10

#define LED1                    27u
#define LED2                    26u
#define GPIO_OUTPUT_PIN_SEL     ((1ULL << LED1) |\
                                 (1ULL << LED2))

#define LEDC_TEST_CH_NUM       (3)
#define LEDC_HS_CH0_GPIO       (26)
#define LEDC_HS_CH1_GPIO       (27)
#define LEDC_HS_CH2_GPIO       (33)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0


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
    ledc_timer_config(ledc_timer);
    ledc_channel_config(ledc_channel);
    ledc_fade_func_install(0);

}

static void PWM_method(ledc_channel_config_t *ledc_channel, int16_t  duty, int16_t  i) {
    duty = -duty;
    duty = (duty < -250) ? -250 : duty;
    duty = (duty > 250) ? 250 : duty;
    duty += 252;
    duty = duty / 2;
    printf("duty = %d\n" ,duty);

    if (!ledc_channel)
        return;
    ledc_set_fade_with_time(ledc_channel->speed_mode,
                            ledc_channel->channel, duty, FADE_TIME);
    ledc_fade_start(ledc_channel->speed_mode,
                    ledc_channel->channel, LEDC_FADE_WAIT_DONE);
    vTaskDelay(30 / portTICK_PERIOD_MS);
}

void pwm_leds(int16_t duty, int16_t i) {
    ledc_timer_config_t ledc_timer[LEDC_TEST_CH_NUM];
    ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM];
    init_timer_chanel(&ledc_timer[0], &ledc_channel[0], LEDC_HS_CH0_GPIO, LEDC_HS_CH0_CHANNEL);
    init_timer_chanel(&ledc_timer[1], &ledc_channel[1], LEDC_HS_CH1_GPIO, LEDC_HS_CH0_CHANNEL);
    init_timer_chanel(&ledc_timer[2], &ledc_channel[2], LEDC_HS_CH2_GPIO, LEDC_HS_CH0_CHANNEL);
    PWM_method(&ledc_channel[0], duty, i);
}