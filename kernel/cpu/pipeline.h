/*
 * OpenOS - 5-Stage Pipelined CPU Simulator
 * 
 * Simulates a classic RISC 5-stage pipeline:
 * IF (Instruction Fetch)
 * ID (Instruction Decode)
 * EX (Execute)
 * MEM (Memory Access)
 * WB (Write Back)
 */

#ifndef OPENOS_PIPELINE_H
#define OPENOS_PIPELINE_H

#include <stdint.h>
#include <stdbool.h>

/* Pipeline stage identifiers */
typedef enum {
    STAGE_IF = 0,   /* Instruction Fetch */
    STAGE_ID = 1,   /* Instruction Decode */
    STAGE_EX = 2,   /* Execute */
    STAGE_MEM = 3,  /* Memory Access */
    STAGE_WB = 4,   /* Write Back */
    STAGE_COUNT = 5
} PipelineStageType;

/* Instruction representation */
typedef struct {
    uint32_t opcode;        /* Operation code */
    uint32_t rd;            /* Destination register */
    uint32_t rs1;           /* Source register 1 */
    uint32_t rs2;           /* Source register 2 */
    uint32_t immediate;     /* Immediate value */
    uint32_t pc;            /* Program counter for this instruction */
    bool valid;             /* Whether this stage contains valid instruction */
} Instruction;

/* Pipeline stage state */
typedef struct {
    Instruction instr;      /* Instruction in this stage */
    bool stalled;           /* Whether this stage is stalled */
} PipelineStage;

/* Pipelined CPU state */
typedef struct {
    PipelineStage stages[STAGE_COUNT];  /* All 5 pipeline stages */
    uint32_t pc;                        /* Program counter */
    uint32_t registers[32];             /* Register file */
    uint64_t cycle_count;               /* Total cycles executed */
    uint64_t instruction_count;         /* Total instructions completed */
    uint64_t stall_count;               /* Total stalls */
    bool running;                       /* CPU is running */
} PipelineCPU;

/* Initialize pipelined CPU */
void pipeline_init(PipelineCPU *cpu);

/* Execute one clock cycle */
void pipeline_cycle(PipelineCPU *cpu, uint32_t *memory, uint32_t mem_size);

/* Execute N instructions */
void pipeline_execute(PipelineCPU *cpu, uint32_t *memory, uint32_t mem_size, uint32_t num_instructions);

/* Get pipeline statistics */
double pipeline_get_cpi(const PipelineCPU *cpu);
uint64_t pipeline_get_cycles(const PipelineCPU *cpu);
uint64_t pipeline_get_instructions(const PipelineCPU *cpu);
uint64_t pipeline_get_stalls(const PipelineCPU *cpu);

/* Hazard detection */
bool pipeline_detect_hazard(const PipelineCPU *cpu);

/* Reset CPU state */
void pipeline_reset(PipelineCPU *cpu);

#endif /* OPENOS_PIPELINE_H */
