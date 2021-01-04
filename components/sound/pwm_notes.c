#include "sound.h"

#define LEDC_CHANNEL_1          3
#define LEDC_LS_CH1_GPIO        25

#define LEDC_LS_CH1_CHANNEL     LEDC_CHANNEL_1
#define LEDC_LS_TIMER           LEDC_TIMER_1
#define LEDC_LS_MODE            LEDC_LOW_SPEED_MODE

#define MIN_DUTY                5

#define B_NOTE                  246
#define Cis_1OCTAL_NOTE         277
#define D_1OCTAL_NOTE           293
#define Fis_1OCTAL_NOTE         369
#define A_1OCTAL_NOTE           440
#define B_1OCTAL_NOTE           493

static void init(ledc_timer_config_t *ledc_timer, ledc_channel_config_t *ledc_channel) {
    *ledc_timer = (ledc_timer_config_t){
            .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
            .freq_hz = B_NOTE,                      // frequency of PWM signal
            .speed_mode = LEDC_LS_MODE,           // timer mode
            .timer_num = LEDC_LS_TIMER,            // timer index
            .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    *ledc_channel = (ledc_channel_config_t){
            .channel    = LEDC_LS_CH1_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH1_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
    };
    ledc_channel_config(ledc_channel);
    ledc_timer_config(ledc_timer);
}


//void pwm_notes(int16_t freq, int16_t  duty,  ledc_channel_config_t *ledc_channel) {
//
//
//    if (duty < 0)
//        duty *= -16;
//    // int delay = 50;
//    // int i = 0xff;
//    // for (i = 0; i < 0xffff; i += 16) {
//        printf("%x\n", DEFAULT_DUTY + duty);
//        ledc_set_duty(*ledc_channel->speed_mode, *ledc_channel->channel, DEFAULT_DUTY + duty);
//        ledc_update_duty(*ledc_channel->speed_mode, *ledc_channel->channel);
//        ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, freq);
//        // vTaskDelay(delay /portTICK_PERIOD_MS);
//        // // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, Cis_1OCTAL_NOTE);
//        // // vTaskDelay(delay /portTICK_PERIOD_MS);
//        // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, D_1OCTAL_NOTE);
//        // vTaskDelay(delay /portTICK_PERIOD_MS);
//        // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, Fis_1OCTAL_NOTE);
//        // vTaskDelay(delay /portTICK_PERIOD_MS);
//        // // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, A_1OCTAL_NOTE);
//        // // vTaskDelay(delay /portTICK_PERIOD_MS);
//        // // ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, B_1OCTAL_NOTE);
//    // }
//}



//void note_to_oled(void *arg) {
//    t_app *app = (t_app*)arg;
//    t_display *display = malloc(sizeof(t_display));
//    oled_init(display);
//    uint16_t old_note = 0;
//
//    if (!display)
//        return;
//    if (old_note != app->note) {
//        oled_clear(display);
//        send_to_oled(display, app->note_to_oled, NULL); // NULL - for duty
//        old_note = app->note;
//    }
//}

void pwm_note_task(void *arg) {
    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel;
    init(&ledc_timer, &ledc_channel);
    t_display *display = malloc(sizeof(t_display));
    oled_init(display);
    t_app *app = (t_app *) arg;
    uint16_t old_note = 0;
    int16_t duty = 0;
    uint8_t io_num;

    ledc_set_freq(LEDC_LS_MODE, LEDC_LS_TIMER, 440);
    while (1) {
            duty = app->acclr[1];
            if (duty < 0)
                duty *= -2;
            else
                duty *= 16;
            printf("DUTY = %d\n", duty + DEFAULT_DUTY);
            printf("DUTY = %d\n", duty);
            printf("ACCLR = %d\n", app->acclr[1]);
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
            if (old_note != app->note) {
                oled_clear(display);
                send_to_oled(display, app->note_to_oled, NULL); // NULL - for duty
                old_note = app->note;
            }
        while (xQueueReceive(gpio_button_evt_queue, &io_num, 0)) {
            if (io_num == GPIO_INPUT_IO_0 && app->delay < 60000) {
                app->delay += 50;
            }
            else if (app->delay > 0) {
                app->delay -= 50;
            }
        }
//        printf("\nIN SOUND delay = %d\n", app->delay);
        vTaskDelay(app->delay / portTICK_PERIOD_MS);
//        vTaskDelay(2000/ portTICK_PERIOD_MS);
    }

}
