[ORG 0x00]			; Set the start address to 0x00
[BITS 16]			; Set the below assemblies to 16 bits

SECTION .text		; Define text segment

;;;;;;;;;;;;;;;;
; Code section ;
;;;;;;;;;;;;;;;;
; INITIALISE SEGMENT REGISTERS
					; Bootloader is loaded to 0x7C00 and its size is 512 bytes
					; CS, DS should be set from 0x7C00 to 0X7E00
jmp 0x07C0:START	; Set CS segment register to 0X07C0,
					; and then jump to START label

TOTALSECTORCOUNT:	dw 1	; OS image size
							; maximum 1152 sectors

START:				; Label START
	mov ax, 0x07C0	; Copy 0x07C0(DS segment address) to AX register
	mov ds, ax		; Set the DS segment address
					; in Real mode, accessing data from an address in assemblies
					; is performed through ds value
	mov ax, 0xB800	; Copy 0xB800(the start address of video buffer) to AX
					; register
	mov es, ax		; Set the video buffer address to es register 

; INITIALISE STACK from 0x0000:0000 - 0x0000:FFFF by 64 kbytes
	mov ax, 0x0000	; Copy 0x0000 to AX register
	mov ss, ax		; Set the SS segment address (the start of the stack)
	mov sp, 0xFFFE	; Set SP and BP registers to 0xFFFE
	mov bp, 0xFFFE	; Current and temporary stack pointers

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

; PRINT BOOT LOADER MESSAGE
	push MESSAGE1
	push 0
	push 0
	call PRINTMESSAGE
	add sp, 6	; remove pushed params

;;;;;;;;;;;;;;;;;
; OS IMAGE LOAD ;
;;;;;;;;;;;;;;;;;

; PRINT OS IMAGE LOAD START MESSAGE
	push IMAGELOADINGMESSAGE
	push 1
	push 0
	call PRINTMESSAGE
	add sp, 6

; LOAD OS FROM A DISK
; 1) RESET DISK
RESETDISK:
	; CALL BIOS RESET FUNCTION TO INITIALISE DISK STATUS
	; SERVICE number 0 (reset), drive number 0 (Floppy)
	mov ax, 0
	mov dl, 0
	int 0x13 ; Execute BIOS interrupt handler

	jc HANDLEDISKERROR ; If error occurs, move to error handling

	; READ DISK SECORS
	; Set the start address of OS image to 0x10000
	mov si, 0x1000
	mov es, si	; set ES register to address designation
	mov bx, 0x0000	; set copy offset to 0x0000

	mov di, word [ TOTALSECTORCOUNT ] ; set the number of disk sectors to csopy

; 2) READ DISK
READDATA:
	cmp di, 0
	je READEND
	sub di, 0x1

	; CALL BIOS READ FUNCTION
	mov ah, 0x02	; Set the BIOS service number 2 (Read Sector)
	mov al, 0x01	; Set the number of read sector to 1
	mov ch, byte [ TRACKNUMBER ]	; Set a track number to read
	mov cl, byte [ SECTORNUMBER ]	; Set a sector number
	mov dh, byte [ HEADNUMBER ]		; Set a head number
	mov dl, 0x00 ; Set the number of device
	int 0x13	; Trigger BIOS interrupt
	jc HANDLEDISKERROR ; Error handling

	; Calculate the next disk sector and the address of destination

	; Set the dest address
	add si, 0x0020	; 1 sector (0x200 bytes) was read
	mov es, si		; set es to read the next sector

	; Increment a sector number
	mov al, byte [ SECTORNUMBER ]
	add al, 0x01
	mov byte [ SECTORNUMBER ], al
	cmp al, 19 ; Check if it is necessary to move the next head or track.
	jl READDATA ; If unnecessary, read the next sector

	; Otherwise, toggle a header number and set a sector number to 1
	xor byte [ HEADNUMBER ], 0x01
	mov byte [ SECTORNUMBER ], 0x01
	
	; If the head is changed from 1 to 0, the next track should be read.
	; Increment a track number
	cmp byte [ HEADNUMBER ], 0x00
	jne READDATA

	add byte [ TRACKNUMBER ], 0x01
	jmp READDATA

READEND:
	push LOADINGCOMPLETEMESSAGE
	push 2
	push 0
	call PRINTMESSAGE
	add sp, 6

; 3) EXECUTE LOADED OS
	jmp 0x1000:0x0000

;;;;;;;;;;;;;;;;;;;;
; Code - functions ;
;;;;;;;;;;;;;;;;;;;;
; Function to handle disk error
HANDLEDISKERROR:
	push DISKERRORMESSAGE	; Insert the address of string to stack
	push 1					; Y coordinate 1
	push 20					; X coordinate 20
	call PRINTMESSAGE		; call Function PRINTMESSAGE

	jmp $					; Execute an infinite loop

; Function to print message
;	PARAM: x coordinate, y coordinate, string's address
PRINTMESSAGE:
	push bp		; Push stack base pointer register
	mov bp, sp	; Set the current stack pointer to the base pointer register
	push es		; Save the previous context
	push si
	push di
	push ax
	push cx
	push dx

	mov ax, 0xB800 ; Set es register 0xB800 to access the video buffer(0xB8000 ~)
	mov es, ax

	; Get a location from x and y coordinates
	; Get a vertical potision
	mov ax, word [ bp + 6 ]
	mov si, 160
	mul si
	mov di, ax

	; Get a horizontal position
	mov ax, word [ bp + 4 ]
	mov si, 2
	mul si
	add di, ax

	; Get the address of string to print
	mov si, word [ bp + 8 ]

	; Message print loop
.MESSAGELOOP:
	mov cl, byte [ si ] ; get a character from si

	;if cl is 0, then escape from this loop.
	cmp cl, 0
	je .MESSAGEEND

	; copy the data at cl to 0xB8000 + di
	mov byte [ es : di ], cl

	add si, 1
	add di, 2

	jmp .MESSAGELOOP

.MESSAGEEND:
	; Recover the previous call stack and the base pointer address
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es
	pop bp
	ret

;;;;;;;;;;;;;;;;
; Data section ;
;;;;;;;;;;;;;;;;

; MESSAGES
MESSAGE1:	db 'MYOS BOOT LOADER', 0 ; Define a message
DISKERRORMESSAGE:	db 'DISK ERROR OCCURS', 0
IMAGELOADINGMESSAGE: db 'SYSTEM LOADS...', 0
LOADINGCOMPLETEMESSAGE: db 'LOADING COMPLETE', 0

; VARIABLES
SECTORNUMBER:	db 0x02	; SECTOR NUMBER (0x01 ~ 0x19)
HEADNUMBER:		db 0x00 ; HEAD NUMBER (0x00 ~ 0x02) both sides of disks
TRACKNUMBER:	db 0x00 ; TRACK NUMBER (0x00 ~ 0x80)

times 510 - ( $ - $$) db 0x00 ; From the current to the 510 bytes, set to 0x00

					; Refer db(1byte), dw(2bytes), dd(4bytes), dq(8bytes)
db 0x55				; Set 0x55 510-byte address
db 0xAA				; Set 0xAA 511-byte address
