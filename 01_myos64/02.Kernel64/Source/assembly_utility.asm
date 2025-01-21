[BITS 64]

SECTION .text

; Export function name
global kInPortByte, kOutPortByte

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
