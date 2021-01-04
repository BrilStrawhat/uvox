#ifndef SOUND_H
#define SOUND_H

#include "components.h"
#include "buttons.h"
#include "oled.h"

#define LEDC_CHANNEL_1          3
#define LEDC_LS_CH1_GPIO        25

#define LEDC_LS_CH1_CHANNEL     LEDC_CHANNEL_1
#define LEDC_LS_TIMER           LEDC_TIMER_1
#define LEDC_LS_MODE            LEDC_LOW_SPEED_MODE

#define MIN_DUTY                5

int chromatic_mode(int16_t accel_data,  char *note);
int pentatonic_mode(int16_t acclr_data, char *note);
//void notes(int16_t freq, int16_t  duty);
void pwm_note_task(void *arg);
#endif