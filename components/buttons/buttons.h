#ifndef BUTTONS_H
#define BUTTONS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define GPIO_INPUT_IO_0         39
#define GPIO_INPUT_IO_1         18

xQueueHandle gpio_button_evt_queue;

void buttons_init(void);
void *change_delay_cmd(void *arg);

#endif
