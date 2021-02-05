[ORG 0x7C00] 

mov bp, 0x8000                      ; Setting up stack pointer  
mov sp, bp                          ; and base pointer
mov bx, 0x1fb6                      ; store the value to print in bx
call print_hex                      ; call the function

HEX_OUT: 
    db '0x0000',0

alphabet: 
    db '0123456780abcdef', 0x0      ; storing alphabet and access it like an array

; Hang
jmp $

; print the value of bx as hex
print_hex:
    pusha

    mov cx, bx                      ; store original number
    mov si, alphabet                ; use si as bp for alphabet
    
    shr bx, 12                      ; getting the first 4 bit [1]fb6
    mov al, [si + bx]               ; getting the corresponding char from alphabet
    mov [HEX_OUT+2], al             ; setting the third byte

    mov bx, cx
    shr bx, 8                       ; to get [1f]b6
    and bx, 0x0f                    ; to get 2 byte 1[f]b6        
    mov al, [si + bx]       
    mov [HEX_OUT+3], al

    mov bx, cx
    shr bx, 4
    and bx, 0x0f         
    mov al, [si + bx]  
    mov [HEX_OUT+4], al

    mov si, alphabet  
    mov bx, cx
    and bx, 0x0f
    mov al, [si + bx]  
    mov [HEX_OUT+5], al

    mov bx, HEX_OUT
    call print_string
    popa
    ret

%include "print_string.asm" 

times 510 - ($-$$) db 0
dw 0xaa55