/*
 * OpenOS - Performance Counters
 * 
 * Tracks and computes CPU and memory performance metrics.
 */

#ifndef OPENOS_PERFORMANCE_H
#define OPENOS_PERFORMANCE_H

#include <stdint.h>

/* Performance counter structure */
typedef struct {
    uint64_t cycles;            /* Total clock cycles */
    uint64_t instructions;      /* Total instructions executed */
    uint64_t cache_hits;        /* Cache hits */
    uint64_t cache_misses;      /* Cache misses */
    uint64_t stalls;            /* Pipeline stalls */
} PerformanceCounters;

/* Initialize performance counters */
void performance_init(PerformanceCounters *perf);

/* Update counters */
void performance_update_cycles(PerformanceCounters *perf, uint64_t cycles);
void performance_update_instructions(PerformanceCounters *perf, uint64_t instructions);
void performance_update_cache(PerformanceCounters *perf, uint64_t hits, uint64_t misses);
void performance_update_stalls(PerformanceCounters *perf, uint64_t stalls);

/* Calculate metrics */
double performance_get_cpi(const PerformanceCounters *perf);
double performance_get_ipc(const PerformanceCounters *perf);
double performance_get_mips(const PerformanceCounters *perf, uint64_t clock_freq_mhz);
double performance_get_cache_hit_rate(const PerformanceCounters *perf);
double performance_get_cache_miss_rate(const PerformanceCounters *perf);

/* Print performance summary */
void performance_print_summary(const PerformanceCounters *perf, uint64_t clock_freq_mhz);

#endif /* OPENOS_PERFORMANCE_H */
