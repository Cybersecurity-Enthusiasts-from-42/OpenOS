/*
 * OpenOS - Memory Bus Simulator Implementation
 */

#include "bus.h"
#include <stddef.h>

/* Simple memset implementation for freestanding environment */
static void *simple_memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

/* Initialize bus */
void bus_init(MemoryBus *bus) {
    simple_memset(bus, 0, sizeof(MemoryBus));
    bus->busy = false;
}

/* Reset bus state */
void bus_reset(MemoryBus *bus) {
    bus_init(bus);
}

/* Request bus transaction */
bool bus_request(MemoryBus *bus, BusTransactionType type, uint32_t address, uint32_t size) {
    (void)address;  /* Address not used in simplified simulation */
    
    if (bus->busy) {
        return false;  /* Bus is busy */
    }
    
    /* Start transaction */
    bus->busy = true;
    bus->total_bytes += size;
    
    if (type == BUS_READ) {
        bus->read_transactions++;
    } else {
        bus->write_transactions++;
    }
    
    return true;
}

/* Advance bus one cycle */
void bus_cycle(MemoryBus *bus) {
    bus->cycle_count++;
    
    /* Simplified: assume transaction completes immediately */
    bus->busy = false;
}

/* Calculate memory latency in cycles */
uint64_t bus_get_memory_latency_cycles(void) {
    /* Memory access: 30 ns */
    /* Bus frequency: 800 MHz = 1.25 ns per cycle */
    /* Latency in cycles = 30 ns / 1.25 ns = 24 cycles */
    return (MEMORY_ACCESS_NS * BUS_FREQUENCY_MHZ) / 1000;
}

/* Get memory latency in nanoseconds */
double bus_get_memory_latency_ns(void) {
    return (double)MEMORY_ACCESS_NS;
}

/* Calculate bus throughput in MB/s */
double bus_get_throughput_mbps(const MemoryBus *bus) {
    if (bus->cycle_count == 0) return 0.0;
    
    /* Throughput = (Total bytes / Total cycles) * Frequency */
    /* Result in bytes per second, then convert to MB/s */
    double bytes_per_cycle = (double)bus->total_bytes / (double)bus->cycle_count;
    double bytes_per_second = bytes_per_cycle * BUS_FREQUENCY_MHZ * 1000000.0;
    return bytes_per_second / (1024.0 * 1024.0);
}

/* Calculate bandwidth utilization */
double bus_get_bandwidth_utilization(const MemoryBus *bus) {
    if (bus->cycle_count == 0) return 0.0;
    
    /* Maximum bandwidth = BUS_WIDTH * Frequency */
    /* Maximum bytes per second = 8 bytes * 800 MHz = 6400 MB/s */
    double max_throughput = (double)BUS_WIDTH_BYTES * BUS_FREQUENCY_MHZ;
    double actual_throughput = bus_get_throughput_mbps(bus);
    
    return actual_throughput / max_throughput;
}

/* Get read transactions */
uint64_t bus_get_read_transactions(const MemoryBus *bus) {
    return bus->read_transactions;
}

/* Get write transactions */
uint64_t bus_get_write_transactions(const MemoryBus *bus) {
    return bus->write_transactions;
}

/* Get total bytes transferred */
uint64_t bus_get_total_bytes(const MemoryBus *bus) {
    return bus->total_bytes;
}

/* Print bus statistics (stub for freestanding environment) */
void bus_print_stats(const MemoryBus *bus) {
    /* In a real kernel, we would print to console */
    (void)bus;
}
