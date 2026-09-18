#include <paging.h>
#include <pmm.h>
#include <stdint.h>

// Page directory and first page table (aligned to 4KB)
__attribute__((aligned(4096))) static uint32_t page_directory[1024];
__attribute__((aligned(4096))) static uint32_t first_page_table[1024];

void paging_init(void) {
    // Identity map the first 4MB of memory (1024 * 4KB pages)
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    // Set up the first page directory entry to point to our page table
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    // Clear the rest of the page directory entries
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0 | PAGE_RW; // supervisor, read/write, not present
    }

    // Load page directory and enable paging via inline assembly
    __asm__ volatile(
        "mov %0, %%eax\n\t"
        "mov %%eax, %%cr3\n\t"
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t" // Set PG bit (bit 31)
        "mov %%eax, %%cr0\n\t"
        :
        : "r"(page_directory)
        : "%eax"
    );
}

