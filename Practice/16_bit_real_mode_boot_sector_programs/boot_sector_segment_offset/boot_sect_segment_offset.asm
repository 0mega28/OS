;
; A simple boot sector program that demonstrates segment offsetting
;

mov ah, 0x0e                  ; int 10/ ah = 0 eh -> scrolling teletype BIOS routine
mov al, [ the_secret ]
int 0x10

mov bx, 0x7c0
mov ds, bx                     ; We can't directly say mov dx, 0x7c0
mov al, [the_secret]           ; Since ds is defined so effective address will be ds*16+the_secret 
int 0x10

mov al, [es:the_secret]        ; Use ex as offset es*16+the_secret
int 0x10

mov bx, 0x7c0
mov es, bx
mov al, [ es : the_secret ]
int 0x10

jmp $

the_secret:
    db "X"

times 510-($-$$) db 0
dw 0xaa55