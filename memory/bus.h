/*
 * OpenOS - Memory Bus Simulator
 * 
 * Simulates a shared memory bus with:
 * - 64-bit data width (8 bytes)
 * - 800 MHz frequency
 * - 30 ns memory access latency
 */

#ifndef OPENOS_BUS_H
#define OPENOS_BUS_H

#include <stdint.h>
#include <stdbool.h>

/* Bus configuration */
#define BUS_WIDTH_BYTES 8           /* 64-bit = 8 bytes */
#define BUS_FREQUENCY_MHZ 800       /* 800 MHz */
#define MEMORY_ACCESS_NS 30         /* 30 ns latency */

/* Bus transaction types */
typedef enum {
    BUS_READ,
    BUS_WRITE
} BusTransactionType;

/* Bus transaction */
typedef struct {
    BusTransactionType type;
    uint32_t address;
    uint32_t size;          /* Size in bytes */
    uint64_t timestamp;     /* Cycle when transaction started */
} BusTransaction;

/* Bus state */
typedef struct {
    uint64_t cycle_count;           /* Current cycle */
    uint64_t read_transactions;     /* Total reads */
    uint64_t write_transactions;    /* Total writes */
    uint64_t total_bytes;           /* Total bytes transferred */
    bool busy;                      /* Bus is busy */
} MemoryBus;

/* Initialize bus */
void bus_init(MemoryBus *bus);

/* Request bus transaction */
bool bus_request(MemoryBus *bus, BusTransactionType type, uint32_t address, uint32_t size);

/* Advance bus one cycle */
void bus_cycle(MemoryBus *bus);

/* Calculate bus metrics */
double bus_get_throughput_mbps(const MemoryBus *bus);
double bus_get_bandwidth_utilization(const MemoryBus *bus);
uint64_t bus_get_memory_latency_cycles(void);
double bus_get_memory_latency_ns(void);

/* Get statistics */
uint64_t bus_get_read_transactions(const MemoryBus *bus);
uint64_t bus_get_write_transactions(const MemoryBus *bus);
uint64_t bus_get_total_bytes(const MemoryBus *bus);

/* Reset bus state */
void bus_reset(MemoryBus *bus);

/* Print bus statistics */
void bus_print_stats(const MemoryBus *bus);

#endif /* OPENOS_BUS_H */
