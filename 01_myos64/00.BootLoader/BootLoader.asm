[ORG 0x00]			; Set the start address to 0x00
[BITS 16]			; Set the below assemblies to 16 bits

SECTION .text		; Define text segment

; INITIALISE SEGMENT REGISTERS
					; Bootloader is loaded to 0x7C00 and its size is 512 bytes
					; CS, DS should be set from 0x7C00 to 0X7E00
jmp 0x07C0:START	; Set CS segment register to 0X07C0,
					; and then jump to START label

START:				; Label START
	mov ax, 0x07C0	; Copy 0x07C0(DS segment address) to AX register
	mov ds, ax		; Set the DS segment address
					; in Real mode, accessing data from an address in assemblies
					; is performed through ds value
	mov ax, 0xB800	; Copy 0xB800(the start address of video buffer) to AX
					; register
	mov es, ax		; Set the video buffer address to es register 

; CLEAR SCREEN 
	mov si, 0		; Initialise SI register

.SCREENCLEARLOOP:	; A loop to clear machine's screen
	mov byte [ es: si ], 0	; ES segment 0xB800:0x0000 -> copy 0 to 0xB8000
							; Set a byte to 0 <=> display null in a location
	mov byte [ es: si + 1 ], 0x0A	; set colours at the above location
									; 0x0A: green colour with black background 
	add si, 2				; move to the next location

	cmp si, 80 * 25 * 2	; Video buffer's size is 80 * 25 * 2. This code checks
						; if colour setting is finished

	jl .SCREENCLEARLOOP	; The value in si register is less than 80 * 25 * 2.
						; This means that the colour setting is not completed,
						; requiring to move .SCREENCLEARLOOP label.

	mov si, 0			; Initialise SI register
	mov di, 0			; Initialise DI register

; BOOT LOADER MESSAGE PRINT
.MESSAGELOOP: ; A lopp to print a message
	mov cl, byte [ si + MESSAGE1 ]	; copy the character at the address of
									; MESSAGE1 to cl register
									; cl register means the low 1-byte of cx
									; register

	cmp cl, 0						; Compare cl with 0.
	je .MESSAGEEND					; That cl is 0 means EOL.
									; Escape this loop <=> exit printing.

	mov byte [ es: di ], cl			; move cl(a character) to 0xBx0000:di
									; print the character

	add si, 1						; move to the next character
	add di, 2						; move to the next location in video buffer

	jmp .MESSAGELOOP				; jump to the start to print the next
									; character
.MESSAGEEND:

jmp $				; while(1) - Execute an infinite loop

MESSAGE1:	db 'MYOS BOOT LOADER', 0 ; Define a message

times 510 - ( $ - $$) db 0x00 ; From the current to the 510 bytes, set to 0x00

					; Refer db(1byte), dw(2bytes), dd(4bytes), dq(8bytes)
db 0x55				; Set 0x55 510-byte address
db 0xAA				; Set 0xAA 511-byte address
