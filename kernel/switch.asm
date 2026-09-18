[BITS 32]
global ctx_switch

ctx_switch:
    pushfd                  ; Push 32-bit EFLAGS (4 bytes)
    pushad                  ; Push all 32-bit GP registers (32 bytes)

    mov eax, [esp + 40]     ; Address of old_esp (4 ret + 4 flags + 32 regs = 40)
    mov [eax], esp          ; Save current stack pointer

    mov edx, [esp + 44]     ; Address of new_esp (40 + 4)
    mov esp, edx            ; Switch to new process stack

    popad                   ; Restore 32-bit GP registers
    popfd                   ; Restore 32-bit EFLAGS
    ret                     ; Jump to entry point / return location
