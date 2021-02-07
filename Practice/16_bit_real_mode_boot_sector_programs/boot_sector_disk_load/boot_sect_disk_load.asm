;
;   Read some sectors from boot disk using disk load function
;
[org 0x7c00]

mov [BOOT_DRIVE], dl            ; BIOS stores our boot drive in DL

mov bp, 0x8000                  ; Setting stack
mov sp, bp                      ;

;mov bx, 0x0000					; Load 5 sectors to 0x0000(ES):0x9000(BX)
;mov es, bx 						;
mov bx, 0x9000 					;
mov dh, 5						; from the boot disk

mov dl, [BOOT_DRIVE]
call disk_load

mov dx, [0x9000] 				; Print out the first loaded word,
call print_hex					; we expect to be 0xdada, stored
								; at address 0x9000

mov dx, [0x9000 + 512]			; Also, print the first word from the 
call print_hex					; 2nd loaded sector: should be 0xface

jmp $

%include "../boot_sector_utility_functions/print_string.asm"
%include "../boot_sector_utility_functions/print_hex.asm"
%include "../boot_sector_utility_functions/disk_load.asm"

BOOT_DRIVE: db 0

times 510-($-$$) db 0
dw 0xaa55

; We know that BIOS will load only the first 512-byte sector from the disk,
; so if we purposely add a few more sectors to our code by repeating some
; familiar numbers, we can prove to ourselfs that we actually loaded those
; additional two sectors from the disk we booted from.

times 256 dw 0xdada
times 256 dw 0xface