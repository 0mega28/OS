;
;   A simple boot sector program that demonstrates stack
;

mov ah, 0x0e

mov bp, 0x8000
mov sp, bp

push 'A'                ; Pushing character on the stack
push 'B'                ; They are pushed as 16-bit value
push 'C'				; So, the most significant bit will be ox00

mov al, [0x7ffa]
int 0x10				; "C"

pop bx					; pop into bx
mov al, bl				; then copy bl to al
int 0x10				; print al

mov al, [0x7ffa]
int 0x10				; Since "C" popped out garbage value gets printed

pop bx				
mov al, bl
int 0x10		

mov al, [0x7ffe]		; Retrieving value from address 77fe=8000-0002 (16 bits)
int 0x10				; "A" gets printed because values are stored as 16bit

jmp $

times 510-($-$$) db 0
dw 0xaa55