#ifndef LEDS_H
#define LEDS_H

#include "components.h"

#define LEDS_DELAY          500

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

int8_t leds_off(t_app *app, char **argv);
void leds_on(void *arg);
//void pwm_leds(int16_t  duty);

#endif