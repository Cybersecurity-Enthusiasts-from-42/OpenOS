/*
 * OpenOS - CPU Pipeline vs Single-Cycle Benchmark
 * 
 * This benchmark compares the performance of the pipelined CPU
 * against the single-cycle CPU reference implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "../kernel/cpu/pipeline.h"
#include "../kernel/cpu/single_cycle.h"
#include "../kernel/cpu/performance.h"
#include "../memory/cache.h"
#include "../memory/bus.h"

#define NUM_INSTRUCTIONS 20000
#define MEMORY_SIZE 8192  /* 32KB of instruction memory */
#define CLOCK_FREQ_MHZ 1000  /* 1 GHz clock */

/* Generate simple test program */
static void generate_test_program(uint32_t *memory, uint32_t size) {
    /* Generate a mix of instructions */
    for (uint32_t i = 0; i < size && i < NUM_INSTRUCTIONS; i++) {
        uint32_t instr = 0;
        
        /* Mix of instruction types */
        switch (i % 4) {
            case 0:  /* ADD r1, r2, r3 */
                instr = 0x33 | (1 << 7) | (2 << 15) | (3 << 20);
                break;
            case 1:  /* ADDI r4, r5, 10 */
                instr = 0x13 | (4 << 7) | (5 << 15) | (10 << 20);
                break;
            case 2:  /* Load */
                instr = 0x03 | (6 << 7) | (7 << 15) | (4 << 20);
                break;
            case 3:  /* Store */
                instr = 0x23 | (8 << 15) | (9 << 20) | (4 << 7);
                break;
        }
        
        memory[i] = instr;
    }
}

/* Run pipelined CPU benchmark */
static void benchmark_pipeline(uint32_t *memory, uint32_t mem_size) {
    printf("=== Pipelined CPU Benchmark ===\n");
    
    PipelineCPU cpu;
    Cache cache;
    MemoryBus bus;
    PerformanceCounters perf;
    
    pipeline_init(&cpu);
    cache_init(&cache);
    bus_init(&bus);
    performance_init(&perf);
    
    clock_t start = clock();
    
    /* Execute instructions */
    pipeline_execute(&cpu, memory, mem_size, NUM_INSTRUCTIONS);
    
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    
    /* Collect statistics */
    uint64_t cycles = pipeline_get_cycles(&cpu);
    uint64_t instructions = pipeline_get_instructions(&cpu);
    uint64_t stalls = pipeline_get_stalls(&cpu);
    double cpi = pipeline_get_cpi(&cpu);
    
    /* Update performance counters */
    performance_update_cycles(&perf, cycles);
    performance_update_instructions(&perf, instructions);
    performance_update_stalls(&perf, stalls);
    
    /* Calculate MIPS */
    double mips = performance_get_mips(&perf, CLOCK_FREQ_MHZ);
    
    /* Print results */
    printf("Instructions executed: %llu\n", (unsigned long long)instructions);
    printf("Total cycles: %llu\n", (unsigned long long)cycles);
    printf("Pipeline stalls: %llu\n", (unsigned long long)stalls);
    printf("CPI: %.3f\n", cpi);
    printf("MIPS: %.2f\n", mips);
    printf("Execution time: %.6f seconds\n", elapsed);
    printf("\n");
}

/* Run single-cycle CPU benchmark */
static void benchmark_single_cycle(uint32_t *memory, uint32_t mem_size) {
    printf("=== Single-Cycle CPU Benchmark ===\n");
    
    SingleCycleCPU cpu;
    PerformanceCounters perf;
    
    single_cycle_init(&cpu);
    performance_init(&perf);
    
    clock_t start = clock();
    
    /* Execute instructions */
    single_cycle_execute(&cpu, memory, mem_size, NUM_INSTRUCTIONS);
    
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    
    /* Collect statistics */
    uint64_t cycles = single_cycle_get_cycles(&cpu);
    uint64_t instructions = single_cycle_get_instructions(&cpu);
    double cpi = single_cycle_get_cpi(&cpu);
    
    /* Update performance counters */
    performance_update_cycles(&perf, cycles);
    performance_update_instructions(&perf, instructions);
    
    /* Calculate MIPS */
    double mips = performance_get_mips(&perf, CLOCK_FREQ_MHZ);
    
    /* Print results */
    printf("Instructions executed: %llu\n", (unsigned long long)instructions);
    printf("Total cycles: %llu\n", (unsigned long long)cycles);
    printf("CPI: %.3f\n", cpi);
    printf("MIPS: %.2f\n", mips);
    printf("Execution time: %.6f seconds\n", elapsed);
    printf("\n");
}

/* Run cache benchmark */
static void benchmark_cache(void) {
    printf("=== Cache Performance Benchmark ===\n");
    
    Cache cache;
    cache_init(&cache);
    
    /* Simulate memory accesses */
    uint8_t data;
    for (uint32_t i = 0; i < 10000; i++) {
        /* Mix of sequential and random accesses */
        uint32_t addr;
        if (i % 3 == 0) {
            addr = (i * 4) % (CACHE_NUM_LINES * CACHE_BLOCK_SIZE);
        } else {
            addr = (i * 137) % (CACHE_NUM_LINES * CACHE_BLOCK_SIZE * 4);
        }
        
        cache_access(&cache, addr, &data, false);
    }
    
    /* Print cache statistics */
    printf("Cache accesses: %llu\n", (unsigned long long)cache_get_accesses(&cache));
    printf("Cache hits: %llu\n", (unsigned long long)cache_get_hits(&cache));
    printf("Cache misses: %llu\n", (unsigned long long)cache_get_misses(&cache));
    printf("Hit rate: %.2f%%\n", cache_get_hit_rate(&cache) * 100.0);
    printf("Miss rate: %.2f%%\n", cache_get_miss_rate(&cache) * 100.0);
    printf("\n");
}

/* Run bus benchmark */
static void benchmark_bus(void) {
    printf("=== Memory Bus Performance ===\n");
    
    MemoryBus bus;
    bus_init(&bus);
    
    /* Simulate bus transactions */
    for (int i = 0; i < 1000; i++) {
        bus_request(&bus, (i % 2 == 0) ? BUS_READ : BUS_WRITE, i * 4, 8);
        bus_cycle(&bus);
    }
    
    printf("Bus frequency: %d MHz\n", BUS_FREQUENCY_MHZ);
    printf("Bus width: %d bytes\n", BUS_WIDTH_BYTES);
    printf("Memory latency: %llu cycles (%.1f ns)\n",
           (unsigned long long)bus_get_memory_latency_cycles(),
           bus_get_memory_latency_ns());
    printf("Read transactions: %llu\n", (unsigned long long)bus_get_read_transactions(&bus));
    printf("Write transactions: %llu\n", (unsigned long long)bus_get_write_transactions(&bus));
    printf("Total bytes: %llu\n", (unsigned long long)bus_get_total_bytes(&bus));
    printf("Throughput: %.2f MB/s\n", bus_get_throughput_mbps(&bus));
    printf("\n");
}

int main(void) {
    printf("OpenOS CPU Architecture Simulator\n");
    printf("==================================\n\n");
    
    /* Allocate instruction memory */
    uint32_t *memory = (uint32_t *)calloc(MEMORY_SIZE, sizeof(uint32_t));
    if (!memory) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }
    
    /* Generate test program */
    generate_test_program(memory, MEMORY_SIZE);
    
    /* Run benchmarks */
    benchmark_pipeline(memory, MEMORY_SIZE);
    benchmark_single_cycle(memory, MEMORY_SIZE);
    benchmark_cache();
    benchmark_bus();
    
    /* Cleanup */
    free(memory);
    
    printf("Benchmark completed successfully!\n");
    return 0;
}
