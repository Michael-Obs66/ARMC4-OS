#ifndef IPC_H
#define IPC_H

#include <stdint.h>

int ipc_queue_create(uint32_t size);
int ipc_send(int queue_id, void *message, uint32_t size);
int ipc_receive(int queue_id, void *buffer, uint32_t size);
int ipc_semaphore_create(uint32_t initial_value);
void ipc_semaphore_wait(int sem_id);
void ipc_semaphore_signal(int sem_id);

#endif
