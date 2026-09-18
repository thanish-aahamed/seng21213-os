#include <idt.h>

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load(uint32_t);
extern void default_isr_stub(void);

void default_int_handler(void) {
    __asm__ volatile ("outb %0, %1" : : "a"((uint8_t)0x20), "Nd"((uint16_t)0x20));
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, (uint32_t)default_isr_stub, 0x08, 0x8E);
    }

    idt_load((uint32_t)&idtp);
}
