;
;   A simple boot sector program that loops forever
;

loop:                           ; Define a label 'loop' (We'll jump to it)

    jmp loop                    ; Jump to "loop" label
                                ; In our case jump to address of current instruction

times 510-($-$$) db 0           ; After compiling, our program must fit into 512 bytes
                                ; and the last two bytes must be the magic number 0xaa55 (Little Endian)

dw 0xaa55                       ; The magic number so that the BIOS knows we are a boot sector