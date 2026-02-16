/*
 * OpenOS - Performance Counters Implementation
 */

#include "performance.h"
#include <stddef.h>

/* Simple memset implementation for freestanding environment */
static void *simple_memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

/* Initialize performance counters */
void performance_init(PerformanceCounters *perf) {
    simple_memset(perf, 0, sizeof(PerformanceCounters));
}

/* Update cycles */
void performance_update_cycles(PerformanceCounters *perf, uint64_t cycles) {
    perf->cycles += cycles;
}

/* Update instructions */
void performance_update_instructions(PerformanceCounters *perf, uint64_t instructions) {
    perf->instructions += instructions;
}

/* Update cache statistics */
void performance_update_cache(PerformanceCounters *perf, uint64_t hits, uint64_t misses) {
    perf->cache_hits += hits;
    perf->cache_misses += misses;
}

/* Update stalls */
void performance_update_stalls(PerformanceCounters *perf, uint64_t stalls) {
    perf->stalls += stalls;
}

/* Calculate CPI (Cycles Per Instruction) */
double performance_get_cpi(const PerformanceCounters *perf) {
    if (perf->instructions == 0) return 0.0;
    return (double)perf->cycles / (double)perf->instructions;
}

/* Calculate IPC (Instructions Per Cycle) */
double performance_get_ipc(const PerformanceCounters *perf) {
    if (perf->cycles == 0) return 0.0;
    return (double)perf->instructions / (double)perf->cycles;
}

/* Calculate MIPS (Million Instructions Per Second) */
double performance_get_mips(const PerformanceCounters *perf, uint64_t clock_freq_mhz) {
    if (perf->cycles == 0) return 0.0;
    
    /* MIPS = (Instructions / Cycles) * Clock Frequency (MHz) */
    double ipc = performance_get_ipc(perf);
    return ipc * clock_freq_mhz;
}

/* Calculate cache hit rate */
double performance_get_cache_hit_rate(const PerformanceCounters *perf) {
    uint64_t total = perf->cache_hits + perf->cache_misses;
    if (total == 0) return 0.0;
    return (double)perf->cache_hits / (double)total;
}

/* Calculate cache miss rate */
double performance_get_cache_miss_rate(const PerformanceCounters *perf) {
    uint64_t total = perf->cache_hits + perf->cache_misses;
    if (total == 0) return 0.0;
    return (double)perf->cache_misses / (double)total;
}

/* Print performance summary (stub for freestanding environment) */
void performance_print_summary(const PerformanceCounters *perf, uint64_t clock_freq_mhz) {
    /* In a real kernel, we would print to console */
    /* For now, this is a placeholder */
    (void)perf;
    (void)clock_freq_mhz;
}
