/*
 * OpenOS - Symmetric Multi-Processing (SMP) Support
 * 
 * Provides multi-core CPU detection and initialization
 */

#ifndef OPENOS_SMP_H
#define OPENOS_SMP_H

#include <stdint.h>

/* Maximum number of CPUs supported */
#define MAX_CPUS 16

/* CPU states */
typedef enum {
    CPU_STATE_OFFLINE,
    CPU_STATE_ONLINE,
    CPU_STATE_HALTED
} cpu_state_t;

/* Per-CPU data structure */
typedef struct cpu_info {
    uint32_t cpu_id;
    cpu_state_t state;
    uint32_t apic_id;
    uint32_t flags;
    uint64_t tsc_freq;  /* Time-stamp counter frequency */
} cpu_info_t;

/* SMP system information */
typedef struct smp_info {
    uint32_t cpu_count;
    uint32_t bsp_id;  /* Bootstrap Processor ID */
    cpu_info_t cpus[MAX_CPUS];
} smp_info_t;

/* Initialize SMP subsystem */
void smp_init(void);

/* Get number of CPUs */
uint32_t smp_get_cpu_count(void);

/* Get current CPU ID */
uint32_t smp_get_current_cpu(void);

/* Get CPU info */
cpu_info_t* smp_get_cpu_info(uint32_t cpu_id);

/* Boot Application Processor */
int smp_boot_ap(uint32_t cpu_id);

/* Halt a CPU */
void smp_halt_cpu(uint32_t cpu_id);

#endif /* OPENOS_SMP_H */
