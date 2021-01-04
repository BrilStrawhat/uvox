#include "driver/gpio.h"
#include "esp_err.h"
#include "buttons.h"

#define GPIO_INPUT_IO_0         39
#define GPIO_INPUT_IO_1         18
#define GPIO_INPUT_PIN_SEL      ((1ULL<<GPIO_INPUT_IO_0)\
                                | (1ULL<<GPIO_INPUT_IO_1))

#define BUF_LEN                 100

#define ESP_INTR_FLAG_DEFAULT   0 

static void IRAM_ATTR gpio_isr_handler(void *arg) {
    uint32_t gpio_num = (uint32_t)arg;

    xQueueSendFromISR(gpio_button_evt_queue, &gpio_num, NULL);
}

void buttons_init(void) {
    esp_err_t rc = ESP_OK;
    char buf[BUF_LEN];
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;

    if ((rc = gpio_config(&io_conf)) != ESP_OK) {
        printf("line %d: %d, %s\n", (__LINE__ - 1), rc, esp_err_to_name_r(rc, buf, BUF_LEN));
        return;
    }
    if ((rc = gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT)) != ESP_OK) {
        printf("line %d: %d, %s\n", (__LINE__ - 1), rc, esp_err_to_name_r(rc, buf, BUF_LEN));
        return;
    }
    if ((gpio_button_evt_queue = xQueueCreate(10, sizeof(uint32_t))) == NULL) {
        printf("line %d: Not enought heap\n", (__LINE__ - 1));
        return;
    }
    if ((rc = gpio_isr_handler_add(GPIO_INPUT_IO_0,
                                   gpio_isr_handler,
                                   (void*) GPIO_INPUT_IO_0)) != ESP_OK) {
        printf("line %d: %d, %s\n", (__LINE__ - 1), rc, esp_err_to_name_r(rc, buf, BUF_LEN));
        return;
    }
    if ((rc = gpio_isr_handler_add(GPIO_INPUT_IO_1,
                                   gpio_isr_handler,
                                   (void*) GPIO_INPUT_IO_1)) != ESP_OK) {
        printf("line %d: %d, %s\n", (__LINE__ - 1), rc, esp_err_to_name_r(rc, buf, BUF_LEN));
        return;
    }
}

