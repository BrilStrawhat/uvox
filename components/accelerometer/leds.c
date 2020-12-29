//#include "accelerometer.h"
//
//void init_timer_chanel(ledc_timer_config_t *ledc_timer,
//                       ledc_channel_config_t *ledc_channel) {
//    *ledc_timer = (ledc_timer_config_t){
//            .duty_resolution = LEDC_TIMER_8_BIT,
//            .freq_hz = 100,
//            .speed_mode = LEDC_LOW_SPEED_MODE,
//            .timer_num =  LEDC_TIMER_0,
//            .clk_cfg = LEDC_AUTO_CLK,
//    };
//    *ledc_channel = (ledc_channel_config_t){
//            .channel    = LEDC_CHANNEL_0,
//            .duty       = 0,
//            .gpio_num   = GPIO_LED_1,
//            .speed_mode = LEDC_LOW_SPEED_MODE,
//            .hpoint     = 0,
//            .timer_sel  = LEDC_TIMER_0
//    };
//    ledc_timer_config(ledc_timer);
//    ledc_channel_config(ledc_channel);
//    ledc_fade_func_install(0);
//}
//
//void PWM_method(ledc_channel_config_t *ledc_channel) {
//    if (!ledc_channel)
//        return;
//    ledc_set_fade_with_time(ledc_channel->speed_mode,
//                            ledc_channel->channel, MAX_DUTY, FADE_TIME);
//    ledc_fade_start(ledc_channel->speed_mode,
//                    ledc_channel->channel, LEDC_FADE_WAIT_DONE);
//    vTaskDelay(30 / portTICK_PERIOD_MS);
//    ledc_set_fade_with_time(ledc_channel->speed_mode,
//                            ledc_channel->channel, MIN_FADE, FADE_TIME);
//    ledc_fade_start(ledc_channel->speed_mode,
//                    ledc_channel->channel, LEDC_FADE_WAIT_DONE);
//    vTaskDelay(30 / portTICK_PERIOD_MS);
//}