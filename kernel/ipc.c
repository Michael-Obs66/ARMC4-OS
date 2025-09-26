#include "kernel.h"
#include <stdbool.h>
#include "stm32f4xx.h" 
#define MAX_QUEUES 4
#define QUEUE_SIZE 16

typedef struct {
    uint32_t data[QUEUE_SIZE];
    uint32_t head, tail, count;
    bool full;
} message_queue_t;

static message_queue_t queues[MAX_QUEUES];

void ipc_init(void) {
    for (int i = 0; i < MAX_QUEUES; i++) {
        queues[i].head = 0;
        queues[i].tail = 0;
        queues[i].count = 0;
        queues[i].full = false;
    }
}

bool message_send(uint32_t queue_id, uint32_t message) {
    if (queue_id >= MAX_QUEUES) return false;
    
    message_queue_t *queue = &queues[queue_id];
    
    if (queue->full) return false;
    
    queue->data[queue->tail] = message;
    queue->tail = (queue->tail + 1) % QUEUE_SIZE;
    queue->count++;
    queue->full = (queue->tail == queue->head);
    
    return true;
}

bool message_receive(uint32_t queue_id, uint32_t *message) {
    if (queue_id >= MAX_QUEUES) return false;
    
    message_queue_t *queue = &queues[queue_id];
    
    if (queue->count == 0) return false;
    
    *message = queue->data[queue->head];
    queue->head = (queue->head + 1) % QUEUE_SIZE;
    queue->count--;
    queue->full = false;
    
    return true;
}

