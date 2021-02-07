[bits 16]
; Switch to protected mode

switch_to_pm:

cli							; We must switch-off interrupts until we have
							; set-up the protected mode interrupts vector
							; otherwise interrupts will run riot.

lgdt [gdt_descriptor]		; Load our global descriptor table, which define
							; the protected mode segments (e.g. for code and data)

mov eax, cr0				; To make the switch to protected mode, we set
or eax, 0x1					; the first bit of CR0, a control register
mov cr0, eax

jmp 	CODE_SEG:init_pm	; Make a far jump (i.e. ot a new segment) to our 32 bit
							; code. This also forces the CPU to flush the cache of 
							; pre-fetched and real-mode decoded instructions, which can
							; cause problems.
							; far jump also sets CS(code segment) to CODE_SEG
							; by definition of far jump CPU automatically sets CS
[bits 32]

; Initialise registers and the stack once in PM.
init_pm:
	mov ax, DATA_SEG		; Now in PM, our old segments are meaningless,
	mov ds, ax				; so we point our segment registers to the
	mov ss, ax				; data selector we defined in our GDT
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000		; Update our stack position so it is right 
	mov esp, ebp 			; at the top of free space

	call BEGIN_PM			; Finally, call some well-known label