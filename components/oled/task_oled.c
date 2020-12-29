//#include "oled.h"
//
//
//void data_to_oled(void *arg) {
//    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT));
//    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_32, 1));
//    t_app *apps = (t_app*)arg;
//    t_display *display = malloc(sizeof(t_display));
//    if(!display)
//        return;
//    display->font_str = (uint8_t*)malloc(sizeof(uint8_t) * (128 * 8));
//    bzero(display->font_str, SCREEN_SIZE);
//    display->addr = I2C_ADDR;
//    display->port = I2C_PORT;
//    oled_config(display);
//    oled_clear(display);
//    oled_update(display, NULL, NULL);
//
//    while (1) {
//            oled_clear(display);
//            if(apps->time && apps->temp_hum)
//                send_to_oled(display, apps->time, apps->temp_hum);
//            if(apps->time && apps->alarm && !strcmp(apps->time, apps->alarm)) {
//                make_some_noise("on");
//            }
//            vTaskDelay( 10/ portTICK_PERIOD_MS);
//        }
//}