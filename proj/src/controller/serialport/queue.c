#include <lcom/lcf.h>

#include "queue.h"

Queue* (create_queue)(){
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    if(queue == NULL){
        return NULL;
    }
    queue->capacity = 50;
    queue->front_index = 0;
    queue->back_index = 49;
    queue->size = 0;
    queue->characters = (int*) malloc(queue->capacity * sizeof(int));
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

void (enqueue)(Queue* queue, int character){
    if(queue->size == queue->capacity){
        return;
    }
    queue->back_index = (queue->back_index + 1) % queue->capacity; //even if the back_index has reached the end of the queue (i.e., it's equal to queue->capacity - 1), there might still be empty spaces at the front of the queue where elements have been removed.
    //eu faço isto porque vai ter indice do 0 ao 49 quando chegar ao 49 passa para 0 que ja pode estar livre
    queue->characters[queue->back_index] = character;
    queue->size++;
}

int (dequeue)(Queue* queue){
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

int (queue_read_int)(Queue* queue) {
    int res = 0;
    for (int i = 0; i < 4; i++) {
        int temp = dequeue(queue);
        printf("Read Byte %x\n", temp);
        temp <<= 8*i;
        res += temp;
    }
    return res;
}
