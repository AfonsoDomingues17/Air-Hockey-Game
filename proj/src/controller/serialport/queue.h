#ifndef _LCOM_QUEUE_H_
#define _LCOM_QUEUE_H_

#include <lcom/lcf.h>

typedef struct {
    int front_index, back_index, size;
    int capacity;
    uint8_t* characters;
}Queue;

/**
 * @brief Allocate memory for queue and initialize variables
 * 
 * @return Pointer for created queue.
*/
Queue* (create_queue)();

/**
 * @brief Free allocated queue memory
 * 
 * @param queue Pointer for queue object to be freed.
*/
void (destroy_queue)(Queue* queue);

/**
 * @brief Add uint8_t value to end of queue
 * 
 * @param queue Queue pointer for the queue to add data to.
 * @param character Data to be added to queue.
 * 
 * @return main buffer
*/
void (enqueue)(Queue* queue, uint8_t character);

/**
 * @brief Removes data from front of queue.
 * 
 * @param queue Queue pointer for the queue to add data to.
 * 
 * @return Data removed from 
*/
uint8_t (dequeue)(Queue* queue);

/**
 * @brief Tells caller if queue is empty or not.
 * 
 * @param queue Queue pointer for the queue in question
 * 
 * @return True if queue is empty or false if it is not
*/
bool (is_empty)(Queue* queue);

/**
 * @brief Tells caller how many items the queue has.
 * 
 * @param queue Queue pointer for the queue in question
 * 
 * @return Number of elements in queue
*/
int (queue_get_size)(Queue* queue);

/**
 * @brief Tells caller if queue is full or not.
 * 
 * @param queue Queue pointer for the queue in question
 * 
 * @return True if queue is full or false if it is not
*/
bool(queue_is_full)(Queue* queue);

/**
 * @brief Tells caller queue capacity (maximum size).
 * 
 * @param queue Queue pointer for the queue in question
 * 
 * @return Number of elements queue can have
*/
int(queue_get_capacity)(Queue* queue);
#endif
