#include <process.h>
#include "vga.h"

static process_t process_table[MAX_PROCESSES];
static int current_process_idx = 0;
static int total_processes = 0;

void process_init(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = i;
        process_table[i].state = PROCESS_TERMINATED;
    }
    
    process_table[0].state = PROCESS_RUNNING;
    total_processes = 1;
}

int process_create(void (*entry_point)(void)) {
    if (total_processes >= MAX_PROCESSES) return -1;

    int idx = total_processes;
    process_t *p = &process_table[idx];
    p->pid = idx;
    p->state = PROCESS_READY;

    uint32_t *sp = (uint32_t *)(p->stack + STACK_SIZE);

    // 1. Entry point (popped by ret in ctx_switch)
    *(--sp) = (uint32_t)entry_point;

    // 2. Initial EFLAGS with IF=1 (0x202) (popped by popfd)
    *(--sp) = 0x202;

    // 3. 8 dummy registers for EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX (popped by popad)
    for (int i = 0; i < 8; i++) {
        *(--sp) = 0;
    }

    p->esp = (uint32_t)sp;
    total_processes++;
    return idx;
}

void schedule(void) {
    if (total_processes <= 1) return;

    int prev_idx = current_process_idx;
    int next_idx = (current_process_idx + 1) % total_processes;

    while (process_table[next_idx].state != PROCESS_READY && next_idx != prev_idx) {
        next_idx = (next_idx + 1) % total_processes;
    }

    if (next_idx == prev_idx) return;

    process_table[prev_idx].state = PROCESS_READY;
    process_table[next_idx].state = PROCESS_RUNNING;
    current_process_idx = next_idx;

    ctx_switch(&process_table[prev_idx].esp, process_table[next_idx].esp);
}

void yield(void) {
    schedule();
}
