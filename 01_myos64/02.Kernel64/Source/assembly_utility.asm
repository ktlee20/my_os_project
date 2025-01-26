[BITS 64]

; Export function name
global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR

SECTION .text
; Read a byte from a port
;	PARAM: port number
kInPortByte:
	push rdx

	mov rdx, rdi
	mov rax, 0
	in al, dx

	pop rdx
	ret

; Write a byte to a port
;	PARAM: port number, data
kOutPortByte:
	push rdx
	push rax

	mov rdx, rdi
	mov rax, rsi
	out dx, al

	pop rax
	pop rdx
	ret

; Set GDT table address to GDTR register
; parameters: the address of GDT table information
kLoadGDTR:
	lgdt [rdi] ; Load parameter1(the address of GDTR) to processor
	ret

; Set TSS segment descriptor to TR register
; parameters: the offset of TSS segment descriptor
kLoadTR:
	ltr di ; Load parameter1(the offset of Tss segment descriptor) to processor
	ret

; Set IDT table address to IDTR register
; parameters: the address of idt table information
kLoadIDTR:
	lidt [rdi] ; Load parameter1(the address of IDTR) to processor
	ret
