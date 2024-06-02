#include <lcom/lcf.h>

#include "queue.h"

Queue* (create_queue)(){
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    if(queue == NULL){
        return NULL;
    }
    queue->capacity = 100;
    queue->front_index = 0;
    queue->back_index = 99;
    queue->size = 0;
    queue->characters = (uint8_t*) malloc(queue->capacity * sizeof(uint8_t));
    if(queue->characters == NULL){
        free(queue);
        return NULL;
    }
    return queue;
}


void (destroy_queue)(Queue* queue){
    free(queue->characters);
    free(queue);
}

void (enqueue)(Queue* queue, uint8_t character){
    if(queue->size == queue->capacity){
        return;
    }
    queue->back_index = (queue->back_index + 1) % queue->capacity; //even if the back_index has reached the end of the queue (i.e., it's equal to queue->capacity - 1), there might still be empty spaces at the front of the queue where elements have been removed.
    //eu faço isto porque vai ter indice do 0 ao 49 quando chegar ao 49 passa para 0 que ja pode estar livre
    queue->characters[queue->back_index] = character;
    queue->size++;
}

uint8_t (dequeue)(Queue* queue){
    if(queue->size == 0){
        return 1;
    }
    int front_element = queue->characters[queue->front_index];
    queue->front_index = (queue->front_index + 1) % queue->capacity;
    queue->size--;

    return front_element;
}

bool (is_empty)(Queue* queue){
    return queue->size == 0;
}

int (queue_get_size)(Queue* queue) {
    return queue->size;
}

bool(queue_is_full)(Queue* queue) {
    return queue->size == queue->capacity;
}

int(queue_get_capacity)(Queue* queue) {
    return queue->capacity;
}
