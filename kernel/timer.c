#include <timer.h>
#include <process.h>
#include <idt.h>

extern void irq0_stub(void);

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void irq0_handler(void) {
    outb(0x20, 0x20);
    schedule();
}

void timer_init(uint32_t frequency) {
    idt_set_gate(32, (uint32_t)irq0_stub, 0x08, 0x8E);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFE); // Enable IRQ0 only
    outb(0xA1, 0xFF);

    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}
