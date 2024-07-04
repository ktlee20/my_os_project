[ORG 0x00]	; the start address of this code
[BITS 16]	; Set the below codes to 16 bits

SECTION .text	; Define text section

;;;;;;;;;;;;;;;;
; CODE SECTION ;
;;;;;;;;;;;;;;;;
START:
	mov ax, 0x1000	; the start address of protected mode (0x10000)
	mov ds, ax		; Set the address to segment registers
	mov es, ax

	; Load GDTR
	cli				; Block interrupts
	lgdt [ GDTR ]	; Load GDTR structure to processor

	; Enter protected modes
	; Set CR0 register to enable protected mode
	; For convenience, disable paging, cache, align check and
	; utilise internal FPU
	mov eax, 0x4000003B ; PG=0, CD=1, NW=0, AM=0, WP=0, NE=1, ET=1, TS=1, EM=0,
						; MP=1, PE(projected mode bit)=1
	mov cr0, eax		; Set CR0 register to the above value (Enter protected)

	; Change the kernel code segment to 0x00 base
	; CS segment selector : EIP: set cs register to the code descriptor and 
	; jump to the start of the protected mode
	jmp dword 0x08: ( PROTECTEDMODE - $$ + 0x10000 )

	; Enter the protected mode
[ BITS 32 ]	; Set the below codes to 32 bits
PROTECTEDMODE:
	mov ax, 0x10	; Set the ds, es, fs, gs register to data descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Set the stack from 0x00000000 to 0x0000FFFF by 64-byte size
	mov ss, ax
	mov esp, 0xFFFE
	mov ebp, 0xFFFE

	; Print messages to notify the entry of protected mode
	; Call PRINTMESSAGE FUNCTION
	push ( SWITCHSUCCESSMESSAGE - $$ + 0x10000 )
	push 3
	push 0
	call PRINTMESSAGE
	add esp, 12	; Clean up the stack

	jmp $	; Execute an infinite loop

;;;;;;;;;;;;;;;;;;;;;;;;;
; FUNCTION CODE SECTION ;
;;;;;;;;;;;;;;;;;;;;;;;;;
; PRINTMESSAGEFUNCTION
;	PARAMS: x coordinate, y coordinate, string address
PRINTMESSAGE:
	push ebp		; Callee saves the previous context
	mov ebp, esp
	push esi
	push edi
	push eax
	push ecx
	push edx

	; From Y coordinate, get the vertical location
	mov eax, [ ebp + 12 ]	; get y coordinate
	mov esi, 160			; 80(one line) * 2 bytes
	mul esi					; eax *= 160
	mov edi, eax			; Store the result into edi register

	; From X coordinate, get the horizontal location
	mov eax, [ ebp + 8 ]	; get x coordinate
	mov esi, 2				; 1 byte - location / 1 byte - colours
	mul esi					; eax *= 2
	add edi, eax

	; The address of printed sentences
	mov esi, dword [ ebp + 16 ]

.MESSAGELOOP:	; A loop to print message
	mov cl, byte [ esi ]	; Get a character from string

	cmp cl, 0
	je .MESSAGEEND			; If EOL, escape from this loop

	mov byte [ edi + 0xB8000 ], cl	; Copy cl to video buffer + offset
									; Print a character

	add esi, 1						; Move to the next character
	add edi, 2						; Move to the next buffer location

	jmp .MESSAGELOOP				; Print the next character

.MESSAGEEND:
	pop edx
	pop ecx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret

;;;;;;;;;;;;;;;;
; DATA SECTION ;
;;;;;;;;;;;;;;;;
; Arrange data by 8-byte alignment
align 8, db 0

; Added to align the end of GDTR to 8 bytes.
; 16 bytes padding + GDTR - 48 bytes
dw 0x0000

; Define the data structure of GDTR
GDTR:
	dw GDTEND - GDT - 1			; The size of GDT table
	dd ( GDT - $$ + 0x10000 )	; The start address of GDT table

; DEFINE GDT table
GDT:
	; NULL destriptor table
	NULLDescriptor:
		dw 0x0000
		dw 0x0000
		db 0x00
		db 0x00
		db 0x00
		db 0x00

	; Code segment descriptor for protected mode
	CODEDESCRIPTOR:
		dw 0xFFFF	; Limit [15:0]
		dw 0x0000	; Base[15:0]
		db 0x00		; Base [23:16]
		db 0x9A		; P=1, DPL=0, Code Segment, Execute/Read
		db 0xCF		; G=1, D=1, L=0, Limit [19:16]
		db 0x00		; Base [31:24]

	; Data segment descriptor for protected mode
	DATADESCRIPTOR:
		dw 0xFFFF	; Limit [15:0]
		dw 0x0000	; Base [15:0]
		db 0x00		; Base [23:16]
		db 0x92		; P=1, DPL=0, Data Segment, Read/Write
		db 0xCF		; G=1, D=1, L=0, Limit [19:16]
		db 0x00		; Base [31:24]
GDTEND:

; Message to notify protected mode switching
SWITCHSUCCESSMESSAGE: db 'Succeed switching to protected mode', 0

times 512 - ( $ - $$ ) db 0x00	; Zero-padding for 512-byte aligning
