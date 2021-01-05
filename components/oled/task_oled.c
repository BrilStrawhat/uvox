#include "oled.h"

void data_to_oled(void *arg) {
    t_app *app = (t_app*)arg;
    t_display *display = malloc(sizeof(t_display));
    oled_init(display);
    uint16_t old_note = 0;

    if (!display)
        return;

    while (1) {
        if (old_note != app->note) {
            oled_clear(display);
            send_to_oled(display, app->note_to_oled, NULL); // NULL - for duty
            old_note = app->note;
        }
        vTaskDelay(app->delay/ portTICK_PERIOD_MS);
    }
}
