/*
 * OpenOS - Direct-Mapped Cache Simulator
 * 
 * Simulates a direct-mapped cache with:
 * - 256 lines
 * - 32 bytes per block
 * - 32-bit addressing
 * 
 * Address breakdown:
 * - Offset: 5 bits (0-4) - byte within block (32 bytes = 2^5)
 * - Index: 8 bits (5-12) - cache line (256 lines = 2^8)
 * - Tag: remaining 19 bits (13-31) - identify block
 */

#ifndef OPENOS_CACHE_H
#define OPENOS_CACHE_H

#include <stdint.h>
#include <stdbool.h>

/* Cache configuration */
#define CACHE_NUM_LINES 256
#define CACHE_BLOCK_SIZE 32
#define CACHE_OFFSET_BITS 5
#define CACHE_INDEX_BITS 8
#define CACHE_TAG_BITS 19

/* Cache line structure */
typedef struct {
    bool valid;                         /* Valid bit */
    uint32_t tag;                       /* Tag bits */
    uint8_t data[CACHE_BLOCK_SIZE];     /* Data block */
} CacheLine;

/* Cache structure */
typedef struct {
    CacheLine lines[CACHE_NUM_LINES];   /* All cache lines */
    uint64_t hits;                      /* Total cache hits */
    uint64_t misses;                    /* Total cache misses */
    uint64_t accesses;                  /* Total accesses */
} Cache;

/* Initialize cache */
void cache_init(Cache *cache);

/* Access cache (returns true on hit, false on miss) */
bool cache_access(Cache *cache, uint32_t address, uint8_t *data, bool is_write);

/* Parse address into components */
void cache_parse_address(uint32_t address, uint32_t *tag, uint32_t *index, uint32_t *offset);

/* Get cache statistics */
uint64_t cache_get_hits(const Cache *cache);
uint64_t cache_get_misses(const Cache *cache);
uint64_t cache_get_accesses(const Cache *cache);
double cache_get_hit_rate(const Cache *cache);
double cache_get_miss_rate(const Cache *cache);

/* Reset cache state */
void cache_reset(Cache *cache);

/* Print cache statistics */
void cache_print_stats(const Cache *cache);

#endif /* OPENOS_CACHE_H */
