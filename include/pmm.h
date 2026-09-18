#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>

#define BLOCK_SIZE 4096  // 4 KB per block
#define BLOCKS_PER_BYTE 8

void pmm_init(uint32_t mem_size, uint32_t bitmap_start);
void* pmm_alloc_block();
void pmm_free_block(void* b);

uint32_t pmm_get_total_memory();
uint32_t pmm_get_free_memory();
uint32_t pmm_get_used_memory();

#endif

