;
;   A simple boot sector that prints a message to screen using BIOS routines.
;

mov ah, 0x0e	; Scrolling teletype BIOS routine

mov al, 'H'
int 0x10

mov al, 'e'
int 0x10

mov al, 'l'
int 0x10

mov al, 'l'
int 0x10

mov al, 'o'
int 0x10

jmp $			; Jump to the current addredd (infinite loop)

;
;	Padding and magic BIOS number.
;

times 510-($-$$) db 0

dw 0xaa55