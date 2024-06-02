#ifndef _LCOM_QUEUE_H_
#define _LCOM_QUEUE_H_

#include <lcom/lcf.h>

typedef struct {
    int front_index, back_index, size;
    int capacity;
    uint8_t* characters;
}Queue;

Queue* (create_queue)();
void (destroy_queue)(Queue* queue);
void (enqueue)(Queue* queue, uint8_t character);
uint8_t (dequeue)(Queue* queue);
bool (is_empty)(Queue* queue);
int (queue_get_size)(Queue* queue);
bool(queue_is_full)(Queue* queue);
int(queue_get_capacity)(Queue* queue);
#endif
