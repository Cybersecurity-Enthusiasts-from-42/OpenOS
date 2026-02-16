/*
 * OpenOS - Direct-Mapped Cache Implementation
 */

#include "cache.h"
#include <stddef.h>

/* Simple memset implementation for freestanding environment */
static void *simple_memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

/* Initialize cache */
void cache_init(Cache *cache) {
    simple_memset(cache, 0, sizeof(Cache));
    
    /* Mark all lines as invalid */
    for (int i = 0; i < CACHE_NUM_LINES; i++) {
        cache->lines[i].valid = false;
    }
}

/* Reset cache state */
void cache_reset(Cache *cache) {
    cache_init(cache);
}

/* Parse address into tag, index, and offset */
void cache_parse_address(uint32_t address, uint32_t *tag, uint32_t *index, uint32_t *offset) {
    /* Offset: bits 0-4 (5 bits) */
    *offset = address & 0x1F;
    
    /* Index: bits 5-12 (8 bits) */
    *index = (address >> CACHE_OFFSET_BITS) & 0xFF;
    
    /* Tag: bits 13-31 (19 bits) */
    *tag = address >> (CACHE_OFFSET_BITS + CACHE_INDEX_BITS);
}

/* Access cache (returns true on hit, false on miss) */
bool cache_access(Cache *cache, uint32_t address, uint8_t *data, bool is_write) {
    uint32_t tag, index, offset;
    cache_parse_address(address, &tag, &index, &offset);
    
    cache->accesses++;
    
    CacheLine *line = &cache->lines[index];
    
    /* Check for hit */
    if (line->valid && line->tag == tag) {
        /* Cache hit */
        cache->hits++;
        
        if (is_write && data != NULL) {
            line->data[offset] = *data;
        } else if (!is_write && data != NULL) {
            *data = line->data[offset];
        }
        
        return true;
    }
    
    /* Cache miss */
    cache->misses++;
    
    /* On miss, load block from memory (simulated) */
    line->valid = true;
    line->tag = tag;
    
    /* If writing on miss, update the data */
    if (is_write && data != NULL) {
        line->data[offset] = *data;
    } else if (!is_write && data != NULL) {
        /* Read miss: simulate loading from memory */
        *data = 0;  /* Simplified: return 0 */
    }
    
    return false;
}

/* Get cache hits */
uint64_t cache_get_hits(const Cache *cache) {
    return cache->hits;
}

/* Get cache misses */
uint64_t cache_get_misses(const Cache *cache) {
    return cache->misses;
}

/* Get total accesses */
uint64_t cache_get_accesses(const Cache *cache) {
    return cache->accesses;
}

/* Get hit rate */
double cache_get_hit_rate(const Cache *cache) {
    if (cache->accesses == 0) return 0.0;
    return (double)cache->hits / (double)cache->accesses;
}

/* Get miss rate */
double cache_get_miss_rate(const Cache *cache) {
    if (cache->accesses == 0) return 0.0;
    return (double)cache->misses / (double)cache->accesses;
}

/* Print cache statistics (stub for freestanding environment) */
void cache_print_stats(const Cache *cache) {
    /* In a real kernel, we would print to console */
    (void)cache;
}
