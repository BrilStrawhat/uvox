#ifndef TEST_FIREWARE_H
#define TEST_FIREWARE_H

#define TEST_FIREWARE "tt"
#define TUNER_FIREWARE "tu"

void *test_mode(void* arg);
void *tuner_mode(void* arg);
xQueueHandle test_fireware_queue;

#endif
