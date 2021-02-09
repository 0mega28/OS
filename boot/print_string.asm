;
;   A utility function to print string at address which is stored in bx
;

print_string:
    pusha
    mov ah, 0x0e
    mov si, bx
    jmp print_and_increment

print_and_increment:
    lodsb                   ; Load byte at address DS:(E)SI into AL
    cmp al, 0x00
    je end
    int 0x10
    jmp print_and_increment

end:
    popa
    ret

; Subroutine to print new line
print_nl:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a    ; Newline char
    int 0x10
    mov al, 0x0d    ; Carriage return
    int 0x10

    popa
    ret