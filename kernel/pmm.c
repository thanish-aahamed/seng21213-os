#include <pmm.h>

static uint32_t* memory_bitmap = 0;
static uint32_t total_blocks = 0;
static uint32_t used_blocks = 0;

void pmm_init(uint32_t mem_size, uint32_t bitmap_start) {
    total_blocks = mem_size / BLOCK_SIZE;
    memory_bitmap = (uint32_t*)bitmap_start;
    
    // Clear the bitmap (mark all blocks as free)
    uint32_t bitmap_size = (total_blocks / BLOCKS_PER_BYTE) / sizeof(uint32_t);
    for (uint32_t i = 0; i < bitmap_size; i++) {
        memory_bitmap[i] = 0;
    }
    used_blocks = 0;
}

void* pmm_alloc_block() {
    for (uint32_t i = 0; i < total_blocks; i++) {
        if (!(memory_bitmap[i / 32] & (1 << (i % 32)))) {
            memory_bitmap[i / 32] |= (1 << (i % 32));
            used_blocks++;
            return (void*)(i * BLOCK_SIZE);
        }
    }
    return 0; // Out of memory
}

void pmm_free_block(void* b) {
    uint32_t block_index = (uint32_t)b / BLOCK_SIZE;
    if (memory_bitmap[block_index / 32] & (1 << (block_index % 32))) {
        memory_bitmap[block_index / 32] &= ~(1 << (block_index % 32));
        used_blocks--;
    }
}

uint32_t pmm_get_total_memory() {
    return total_blocks * BLOCK_SIZE;
}

uint32_t pmm_get_free_memory() {
    return (total_blocks - used_blocks) * BLOCK_SIZE;
}

uint32_t pmm_get_used_memory() {
    return used_blocks * BLOCK_SIZE;
}

