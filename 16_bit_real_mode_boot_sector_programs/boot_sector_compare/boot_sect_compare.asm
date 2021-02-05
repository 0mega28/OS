mov bx, 100

cmp bx, 4
jle A
cmp bx, 40
jl B
mov al, 'C'
jmp END

A:
    mov al, 'A'
    jmp END

B:
    mov al, 'B'

END:
    mov ah, 0x0e
    int 0x10

jmp $

times 510-($-$$) db 0
dw 0xaa55
