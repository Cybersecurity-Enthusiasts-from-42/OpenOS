/*
 * OpenOS - Single-Cycle CPU Simulator
 * 
 * Reference implementation: one instruction per cycle.
 */

#include "single_cycle.h"
#include <stddef.h>

/* Simple memset implementation for freestanding environment */
static void *simple_memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

/* Initialize single-cycle CPU */
void single_cycle_init(SingleCycleCPU *cpu) {
    simple_memset(cpu, 0, sizeof(SingleCycleCPU));
    cpu->running = true;
}

/* Reset CPU state */
void single_cycle_reset(SingleCycleCPU *cpu) {
    single_cycle_init(cpu);
}

/* Execute one instruction (one cycle) */
void single_cycle_execute_one(SingleCycleCPU *cpu, uint32_t *memory, uint32_t mem_size) {
    if (!cpu->running) return;
    
    /* Check bounds */
    if (cpu->pc / 4 >= mem_size) {
        cpu->running = false;
        return;
    }
    
    /* Fetch */
    uint32_t raw = memory[cpu->pc / 4];
    
    /* Decode */
    uint32_t opcode = raw & 0x7F;
    uint32_t rd = (raw >> 7) & 0x1F;
    uint32_t rs1 = (raw >> 15) & 0x1F;
    uint32_t rs2 = (raw >> 20) & 0x1F;
    uint32_t immediate = raw >> 20;
    
    /* Execute (simplified) */
    uint32_t result = 0;
    switch (opcode) {
        case 0x33:  /* R-type: ADD/SUB/etc */
            result = cpu->registers[rs1] + cpu->registers[rs2];
            break;
        case 0x13:  /* I-type: ADDI */
            result = cpu->registers[rs1] + immediate;
            break;
        case 0x03:  /* Load */
            if ((cpu->registers[rs1] + immediate) / 4 < mem_size) {
                result = memory[(cpu->registers[rs1] + immediate) / 4];
            }
            break;
        case 0x23:  /* Store */
            if ((cpu->registers[rs1] + immediate) / 4 < mem_size) {
                memory[(cpu->registers[rs1] + immediate) / 4] = cpu->registers[rs2];
            }
            break;
        default:
            result = 0;
            break;
    }
    
    /* Write back */
    if (rd != 0 && opcode != 0x23) {  /* Don't write on store */
        cpu->registers[rd] = result;
    }
    
    /* Update PC */
    cpu->pc += 4;
    cpu->cycle_count++;
    cpu->instruction_count++;
}

/* Execute N instructions */
void single_cycle_execute(SingleCycleCPU *cpu, uint32_t *memory, uint32_t mem_size, uint32_t num_instructions) {
    for (uint32_t i = 0; i < num_instructions && cpu->running; i++) {
        single_cycle_execute_one(cpu, memory, mem_size);
    }
}

/* Get CPI (always 1.0 for single-cycle) */
double single_cycle_get_cpi(const SingleCycleCPU *cpu) {
    if (cpu->instruction_count == 0) return 0.0;
    return (double)cpu->cycle_count / (double)cpu->instruction_count;
}

/* Get total cycles */
uint64_t single_cycle_get_cycles(const SingleCycleCPU *cpu) {
    return cpu->cycle_count;
}

/* Get instructions executed */
uint64_t single_cycle_get_instructions(const SingleCycleCPU *cpu) {
    return cpu->instruction_count;
}
