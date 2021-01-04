#include "accelerometer.h"
#include "test_fireware.h"
#include "uart_console.h"


void *tuner_mode(void* arg) {
    if (arg == NULL || *(char*)arg == '\0') {
        return NULL;
    }
    static uint8_t flags = 0; // BIT0 on/off tuner mode

    if (!strcmp("on", (char*)arg) && (flags & BIT0) == 0) {
        vTaskSuspend(read_acceleration_handler);
        flags |= BIT0;
    }
    else if (!strcmp("off", (char*)arg) && (flags & BIT0) == 1) {
        vTaskResume(read_acceleration_handler);
        flags &= ~BIT0;
    }
    else {
        char *saveptr = NULL;
        char delim = ' ';
        char *freq = NULL;
        char *duty = NULL;

        freq = strtok_r((char*)arg, &delim, &saveptr);
        duty = strtok_r(NULL, &delim, &saveptr);
        if (!freq || !duty) {
            uart_printstr("Wrong arguments");
            return NULL;
        }
        notes(atoi(freq), atoi(duty));
    }
    return NULL;
}


void *test_mode(void* arg) {
    if (arg == NULL || *(char*)arg == '\0') {
        return NULL;
    }
    static uint8_t flags = 0; // BIT0 on/off test mode, BIT1 if queue created 
    int16_t accs[3];

    if (!strcmp("on", (char*)arg) && (flags & BIT0) == 0) {
        xTaskNotifyGive(read_acceleration_handler);
        flags |= BIT0;
        if (!(flags & BIT1)) {
            if ((test_fireware_queue = xQueueCreate(2, sizeof(accs))) == NULL) {
                uart_printstr("Failed to create queue, reboot");
                exit(1);
            }
            else
                flags |= BIT1;
        }
    }
    else if (!strcmp("off", (char*)arg) && (flags & BIT0) == 1) {
        vQueueDelete(test_fireware_queue);
        xTaskNotifyGive(read_acceleration_handler);
        flags &= ~BIT1;
        flags &= ~BIT0;
    }
    else {
        if ((flags & BIT0) == 0)
            return NULL;
        char *saveptr = NULL;
        char delim = ' ';
        char *x = NULL;
        char *y = NULL;
        char *z = NULL;

        x = strtok_r((char*)arg, &delim, &saveptr);
        y = strtok_r(NULL, &delim, &saveptr);
        z = strtok_r(NULL, &delim, &saveptr);
        if (!x || !y || !z) {
            uart_printstr("Wrong arguments");
            return NULL;
        }
        accs[0] = atoi(x);
        accs[1] = atoi(y);
        accs[2] = atoi(z);
        xQueueSend(test_fireware_queue, accs, 0);
    }
    return NULL;
}
