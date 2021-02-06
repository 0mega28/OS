; A boot sector that enters 32-bit protected mode.
[org 0x7c00]

mov bp, 0x9000              ; Set the stack
mov sp, bp


mov bx, MSG_REAL_MODE   
call print_string_pm

call switch_to_pm           ; Note that we never return from here

jmp $

%include "../16_bit_real_mode_boot_sector_programs/boot_sector_utility_functions/print_string.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "switch_to_pm.asm"

[bits 32]
; This is where we arrive after switching to and initialising protected mode
BEGIN_PM:

    mov ebx, MSG_PROT_MODE
    call print_string_pm    ; Use our 32-bit print routine.

    jmp $                   ; Hang

; Gloabal variable
MSG_REAL_MODE db "Started in 16-bit Real mode", 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected mode", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55