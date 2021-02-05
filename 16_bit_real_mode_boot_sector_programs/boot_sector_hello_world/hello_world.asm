;
; A boot sector that prints a string using our function.
;

[org 0x7c00]            ; Tell the assembler where this code will be loaded
                        ; to avoid giving padding manually

; include file where print_string function is written

mov bx, HELLO_MSG
call print_string   

mov bx, GOODBYE_MSG
call print_string

jmp $

%include "print_string.asm" 

; Data
HELLO_MSG:
    db 'Hello, World!',0   ; Zero in the end will tell our 
                            ; routine when to stop printing      
GOODBYE_MSG:
    db 'Goodbye!',0

; Padding and magic number
times 510-($-$$) db 0
dw 0xaa55