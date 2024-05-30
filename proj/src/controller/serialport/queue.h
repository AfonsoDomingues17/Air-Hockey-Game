#ifndef _LCOM_QUEUE_H_
#define _LCOM_QUEUE_H_

#include <lcom/lcf.h>

typedef struct {
    int front_index, back_index, size;
    int capacity;
    int* characters;
}Queue;

Queue* (create_queue)();
void (destroy_queue)(Queue* queue);
void (enqueue)(Queue* queue, int character);
int (dequeue)(Queue* queue);
bool (is_empty)(Queue* queue);
int (queue_read_int)(Queue* queue);

#endif
