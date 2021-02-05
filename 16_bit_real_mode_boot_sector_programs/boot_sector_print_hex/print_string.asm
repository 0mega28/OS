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