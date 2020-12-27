#include "accelerometer.h"

void config_leds_and_beeep() {
    ESP_ERROR_CHECK(dac_output_enable(DAC_CHANNEL_1));
    ESP_ERROR_CHECK(gpio_set_direction(5, GPIO_MODE_OUTPUT)); // AMPLYFIER
    ESP_ERROR_CHECK(gpio_set_direction(5, 1)); // AMPLYFIER
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT)); // ledc
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT));
}

void leds(int value) {
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_26, value));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_27, value));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_33, value));
}

void beeep() {
    int duration = 150;
    int voltage = 255;
    float  frequency = 0.8;
    int delay = 2;

    while (duration > 0) {
        for (float i = 0; i < voltage; i+= frequency) {
            dac_output_voltage(DAC_CHANNEL_1, i);
            ets_delay_us(delay);
        }
        duration--;
        ets_delay_us(10);
    }
    vTaskDelay(10);
}

