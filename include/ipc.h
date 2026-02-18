/*
 * OpenOS - Inter-Process Communication
 * 
 * Provides IPC mechanisms: pipes and message queues
 */

#ifndef OPENOS_IPC_H
#define OPENOS_IPC_H

#include <stdint.h>
#include <stddef.h>

/* Pipe buffer size */
#define PIPE_BUF_SIZE 4096

/* Message queue limits */
#define MSG_QUEUE_SIZE 16
#define MSG_MAX_SIZE 256

/* Pipe structure */
typedef struct pipe {
    uint8_t buffer[PIPE_BUF_SIZE];
    size_t read_pos;
    size_t write_pos;
    size_t count;
    uint32_t reader_pid;
    uint32_t writer_pid;
    int is_open;
} pipe_t;

/* Message structure */
typedef struct message {
    uint32_t sender_pid;
    uint32_t type;
    size_t size;
    uint8_t data[MSG_MAX_SIZE];
} message_t;

/* Message queue structure */
typedef struct msg_queue {
    message_t messages[MSG_QUEUE_SIZE];
    size_t head;
    size_t tail;
    size_t count;
    uint32_t owner_pid;
    int is_open;
} msg_queue_t;

/* IPC initialization */
void ipc_init(void);

/* Pipe operations */
pipe_t* pipe_create(uint32_t reader_pid, uint32_t writer_pid);
int pipe_write(pipe_t* pipe, const void* data, size_t size);
int pipe_read(pipe_t* pipe, void* buffer, size_t size);
void pipe_close(pipe_t* pipe);

/* Message queue operations */
msg_queue_t* msgqueue_create(uint32_t owner_pid);
int msgqueue_send(msg_queue_t* queue, uint32_t sender_pid, uint32_t type, const void* data, size_t size);
int msgqueue_receive(msg_queue_t* queue, message_t* msg);
void msgqueue_close(msg_queue_t* queue);

#endif /* OPENOS_IPC_H */
