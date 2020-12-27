#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_CHANNEL_3         3
#define LEDC_LS_CH3_GPIO       (25)

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_LS_CH3_CHANNEL    LEDC_CHANNEL_3
#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE

#define LEDC_TEST_CH_NUM       (4)
#define LEDC_TEST_DUTY         (20)
#define LEDC_TEST_FADE_TIME    (3000)

void app_main(void)
{
    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 262,                      // frequency of PWM signal
        .speed_mode = LEDC_LS_MODE,           // timer mode
        .timer_num = LEDC_LS_TIMER,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
    ledc_channel_config_t ledc_channel = {
        .channel    = LEDC_LS_CH3_CHANNEL,
        .duty       = 0,
        .gpio_num   = LEDC_LS_CH3_GPIO,
        .speed_mode = LEDC_LS_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_LS_TIMER
    };
    ledc_channel_config(&ledc_channel);

    // Initialize fade service.
    ledc_fade_func_install(0);

    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, LEDC_TEST_DUTY);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    vTaskDelay(1000 /portTICK_PERIOD_MS);
    ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, 293);
    vTaskDelay(1000 /portTICK_PERIOD_MS);
    ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, 293);
    vTaskDelay(1000 /portTICK_PERIOD_MS);
    ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, 329);
    vTaskDelay(1000 /portTICK_PERIOD_MS);
    ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, 349);
    vTaskDelay(1000 /portTICK_PERIOD_MS);
    ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, 391);
    vTaskDelay(1000 /portTICK_PERIOD_MS);
    ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, 440);
    // }
}
