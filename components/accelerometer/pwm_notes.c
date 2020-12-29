#include "accelerometer.h"

#define LEDC_CHANNEL_1          3
#define LEDC_LS_CH1_GPIO        25

#define LEDC_LS_CH1_CHANNEL     LEDC_CHANNEL_1
#define LEDC_LS_TIMER           LEDC_TIMER_1
#define LEDC_LS_MODE            LEDC_LOW_SPEED_MODE

#define DEFAULT_DUTY            0x0
#define MIN_DUTY                5

#define B_NOTE                  246
#define Cis_1OCTAL_NOTE         277
#define D_1OCTAL_NOTE           293
#define Fis_1OCTAL_NOTE         369
#define A_1OCTAL_NOTE           440
#define B_1OCTAL_NOTE           493

void notes(uint16_t note, int i) {
    ledc_timer_config_t ledc_timer = {
            .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
            .freq_hz = B_NOTE,                      // frequency of PWM signal
            .speed_mode = LEDC_LS_MODE,           // timer mode
            .timer_num = LEDC_LS_TIMER,            // timer index
            .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    ledc_channel_config_t ledc_channel = {
            .channel    = LEDC_LS_CH1_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH1_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
    };
    ledc_channel_config(&ledc_channel);
    ledc_timer_config(&ledc_timer);


    int delay = 50;
    // int i = 0xff;
    // for (i = 0; i < 0xffff; i += 16) {
    printf("%x\n", DEFAULT_DUTY + i);
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, DEFAULT_DUTY + i);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, note);
    vTaskDelay(delay /portTICK_PERIOD_MS);
    // // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, Cis_1OCTAL_NOTE);
    // // vTaskDelay(delay /portTICK_PERIOD_MS);
    // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, D_1OCTAL_NOTE);
    // vTaskDelay(delay /portTICK_PERIOD_MS);
    // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, Fis_1OCTAL_NOTE);
    // vTaskDelay(delay /portTICK_PERIOD_MS);
    // // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, A_1OCTAL_NOTE);
    // // vTaskDelay(delay /portTICK_PERIOD_MS);
    // // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, B_1OCTAL_NOTE);
    // }
}

