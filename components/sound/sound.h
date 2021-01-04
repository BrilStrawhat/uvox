#ifndef SOUND_H
#define SOUND_H

#include "components.h"
#include "buttons.h"
#include "oled.h"

int chromatic_mode(int16_t accel_data,  char *note);
int pentatonic_mode(int16_t acclr_data, char *note);
//void notes(int16_t freq, int16_t  duty);
void pwm_note_task(void *arg);
#endif