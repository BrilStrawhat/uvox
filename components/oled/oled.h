#ifndef OLED_H
#define OLED_H


#define OLED_DELAY                          100

#define SDA_PIN                             GPIO_NUM_21
#define SCL_PIN                             GPIO_NUM_22
#define I2C_ADDR                            0x3C
#define I2C_PORT                            I2C_NUM_0
#define SCREEN_WIDTH                        128
#define PAGE_COUNT                          8
#define SCREEN_SIZE                         1024 // width * number of pages


#include "components.h"

void data_to_oled(void *arg);
void send_to_oled(t_display *display, char *str1, char *str2);
void oled_update(t_display *display, char *str1, char *str2);
void oled_clear(t_display *display);
void oled_initial_settings(t_display *display);
void data_to_oled(void *arg);
void oled_config(t_display *display);
void AsciiStr_to_font6x8(char *str, t_display *display, int font_str_intdex);
void oled_init(t_display *display);

#endif