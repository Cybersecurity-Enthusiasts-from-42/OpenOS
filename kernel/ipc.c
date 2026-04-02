/*
 * OpenOS - Inter-Process Communication Implementation
 */

#include "ipc.h"
#include "console.h"
#include "string.h"

/* Static storage for pipes and message queues */
#define MAX_PIPES 32
#define MAX_MSG_QUEUES 32

static pipe_t pipes[MAX_PIPES];
static msg_queue_t msg_queues[MAX_MSG_QUEUES];
static int ipc_initialized = 0;

/* Initialize IPC subsystem */
void ipc_init(void) {
    if (ipc_initialized) return;
    
    /* Initialize all pipes */
    for (int i = 0; i < MAX_PIPES; i++) {
        pipes[i].is_open = 0;
        pipes[i].read_pos = 0;
        pipes[i].write_pos = 0;
        pipes[i].count = 0;
    }
    
    /* Initialize all message queues */
    for (int i = 0; i < MAX_MSG_QUEUES; i++) {
        msg_queues[i].is_open = 0;
        msg_queues[i].head = 0;
        msg_queues[i].tail = 0;
        msg_queues[i].count = 0;
    }
    
    ipc_initialized = 1;
    console_write("IPC: Pipes and message queues initialized\n");
}

/* Create a new pipe */
pipe_t* pipe_create(uint32_t reader_pid, uint32_t writer_pid) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (!pipes[i].is_open) {
            pipes[i].is_open = 1;
            pipes[i].reader_pid = reader_pid;
            pipes[i].writer_pid = writer_pid;
            pipes[i].read_pos = 0;
            pipes[i].write_pos = 0;
            pipes[i].count = 0;
            return &pipes[i];
        }
    }
    return NULL; /* No free pipes */
}

/* Write data to pipe */
int pipe_write(pipe_t* pipe, const void* data, size_t size) {
    if (!pipe || !pipe->is_open || !data) return -1;
    
    const uint8_t* src = (const uint8_t*)data;
    size_t written = 0;
    
    while (written < size && pipe->count < PIPE_BUF_SIZE) {
        pipe->buffer[pipe->write_pos] = src[written];
        pipe->write_pos = (pipe->write_pos + 1) % PIPE_BUF_SIZE;
        pipe->count++;
        written++;
    }
    
    return written;
}

/* Read data from pipe */
int pipe_read(pipe_t* pipe, void* buffer, size_t size) {
    if (!pipe || !pipe->is_open || !buffer) return -1;
    
    uint8_t* dst = (uint8_t*)buffer;
    size_t read = 0;
    
    while (read < size && pipe->count > 0) {
        dst[read] = pipe->buffer[pipe->read_pos];
        pipe->read_pos = (pipe->read_pos + 1) % PIPE_BUF_SIZE;
        pipe->count--;
        read++;
    }
    
    return read;
}

/* Close pipe */
void pipe_close(pipe_t* pipe) {
    if (pipe) {
        pipe->is_open = 0;
    }
}

/* Create a new message queue */
msg_queue_t* msgqueue_create(uint32_t owner_pid) {
    for (int i = 0; i < MAX_MSG_QUEUES; i++) {
        if (!msg_queues[i].is_open) {
            msg_queues[i].is_open = 1;
            msg_queues[i].owner_pid = owner_pid;
            msg_queues[i].head = 0;
            msg_queues[i].tail = 0;
            msg_queues[i].count = 0;
            return &msg_queues[i];
        }
    }
    return NULL; /* No free message queues */
}

/* Send message to queue */
int msgqueue_send(msg_queue_t* queue, uint32_t sender_pid, uint32_t type, const void* data, size_t size) {
    if (!queue || !queue->is_open || size > MSG_MAX_SIZE) return -1;
    if (queue->count >= MSG_QUEUE_SIZE) return -1; /* Queue full */
    
    message_t* msg = &queue->messages[queue->tail];
    msg->sender_pid = sender_pid;
    msg->type = type;
    msg->size = size;
    
    if (data && size > 0) {
        memcpy(msg->data, data, size);
    }
    
    queue->tail = (queue->tail + 1) % MSG_QUEUE_SIZE;
    queue->count++;
    
    return 0;
}

/* Receive message from queue */
int msgqueue_receive(msg_queue_t* queue, message_t* msg) {
    if (!queue || !queue->is_open || !msg) return -1;
    if (queue->count == 0) return -1; /* Queue empty */
    
    /* Copy message */
    memcpy(msg, &queue->messages[queue->head], sizeof(message_t));
    
    queue->head = (queue->head + 1) % MSG_QUEUE_SIZE;
    queue->count--;
    
    return 0;
}

/* Close message queue */
void msgqueue_close(msg_queue_t* queue) {
    if (queue) {
        queue->is_open = 0;
    }
}
