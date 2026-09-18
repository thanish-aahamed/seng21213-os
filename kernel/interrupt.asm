[BITS 32]
global idt_load
global irq0_stub
global default_isr_stub
extern irq0_handler
extern default_int_handler

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

irq0_stub:
    pushad
    pushfd
    call irq0_handler
    popfd
    popad
    iret

default_isr_stub:
    pushad
    pushfd
    call default_int_handler
    popfd
    popad
    iret
