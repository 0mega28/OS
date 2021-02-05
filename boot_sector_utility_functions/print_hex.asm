;
;   A utility function to print content of register dx in hexadecimal format
;

HEX_OUT: 
    db '0x0000',0

alphabet: 
    db '0123456780abcdef', 0x0      ; storing alphabet and access it like an array

; print the value of bx as hex
print_hex:
    pusha

    mov bx, dx
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
    