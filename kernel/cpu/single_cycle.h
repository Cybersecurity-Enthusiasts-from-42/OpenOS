/*
 * OpenOS - Single-Cycle CPU Simulator
 * 
 * Reference implementation with one instruction per cycle.
 * No pipeline, no hazards, simple sequential execution.
 */

#ifndef OPENOS_SINGLE_CYCLE_H
#define OPENOS_SINGLE_CYCLE_H

#include <stdint.h>
#include <stdbool.h>

/* Single-cycle CPU state */
typedef struct {
    uint32_t pc;                        /* Program counter */
    uint32_t registers[32];             /* Register file */
    uint64_t cycle_count;               /* Total cycles = instruction count */
    uint64_t instruction_count;         /* Total instructions executed */
    bool running;                       /* CPU is running */
} SingleCycleCPU;

/* Initialize single-cycle CPU */
void single_cycle_init(SingleCycleCPU *cpu);

/* Execute one instruction (one cycle) */
void single_cycle_execute_one(SingleCycleCPU *cpu, uint32_t *memory, uint32_t mem_size);

/* Execute N instructions */
void single_cycle_execute(SingleCycleCPU *cpu, uint32_t *memory, uint32_t mem_size, uint32_t num_instructions);

/* Get statistics */
double single_cycle_get_cpi(const SingleCycleCPU *cpu);
uint64_t single_cycle_get_cycles(const SingleCycleCPU *cpu);
uint64_t single_cycle_get_instructions(const SingleCycleCPU *cpu);

/* Reset CPU state */
void single_cycle_reset(SingleCycleCPU *cpu);

#endif /* OPENOS_SINGLE_CYCLE_H */
