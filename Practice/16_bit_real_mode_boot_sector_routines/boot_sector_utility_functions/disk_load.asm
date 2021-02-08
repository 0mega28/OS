;
;   Load DH sectors to ES:BX from drive DL
;

disk_load:
    push dx							; Store DX on stack so that later we can recall
									; how many sectors it was meant to read
									; even if it is altered in the meantime
									; No. of sectors read is stored in al after reading
									; we'll compare dx with al to confirm if the read was faulty

	mov ah, 0x02					; BIOS read sector function
	mov al, dh						; Read DH sectors
	mov ch, 0x00					; Select cylinder 0
	mov dh, 0x00					; Select head 0
	mov cl, 0x02					; Start reading from second sector (i.e.) after the boot sector

	int 0x13						; BIOS interrupt to read

	jc disk_error					; If there is an error while read then BIOS sets carry flag(CF)
									; jump only if the CF is set

	pop dx 							; Restore DX from stack
	cmp dh, al 						; if AL(sector read) != DH(sectors expected to read)
	jne disk_error 					; display error message
	ret

disk_error:
	mov bx, DISK_ERROR_MSG
	call print_string
	jmp $

; Variables
DISK_ERROR_MSG:	db "Disk read error!", 0														      	  
