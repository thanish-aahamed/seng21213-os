#ifndef PROCESS_H
#define PROCESS_H

#include <types.h>

#define MAX_PROCESSES 4
#define STACK_SIZE 4096

typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_TERMINATED
} process_state_t;

typedef struct {
    uint32_t pid;
    uint32_t esp;                 // Stack pointer
    uint8_t stack[STACK_SIZE];   // Kernel stack space
    process_state_t state;
} process_t;

extern void ctx_switch(uint32_t *old_esp, uint32_t new_esp);

void process_init(void);
int process_create(void (*entry_point)(void));
void schedule(void);
void yield(void);

#endif
