;
; A simple boot sector program that demonstrates addressing.
;
[org 0x7c00]
mov ah, 0x0e ; int 10/ ah = 0 eh -> scrolling teletype BIOS routine

; First attempt
mov al, "1"
int 0x10
mov al, the_secret
int 0x10   ; Does this print an X?

; Second attempt
mov al, "2"
int 0x10
mov al, [the_secret]
int 0x10   ; Does this print an X?

; Third attempt
mov al, "3"
int 0x10
mov bx, the_secret
add bx, 0x7c00
mov al, [bx]
int 0x10   ; Does this print an X?

; Fourth attempt
mov al, "4"
int 0x10
mov al, [0x7c2d]
int 0x10   ; Does this print an X?

jmp $       ; Jump forever.

the_secret :
    db "X"

; Padding and magic BIOS number.

times 510 -($-$$) db 0
dw 0xaa55