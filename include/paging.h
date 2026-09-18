#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x4

void paging_init(void);
void switch_page_directory(uint32_t* page_directory);
void page_fault_handler(void);

#endif

