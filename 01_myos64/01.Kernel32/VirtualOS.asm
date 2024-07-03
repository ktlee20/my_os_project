[ORG 0x00]	; start address=0x00
[BITS 16]	; using 16 bits code

SECTION .text	; text section

jmp 0x1000:START	; Copy 0x1000 to cs register and Jump to START label

SECTORCOUNT:	dw 0x0000		; Store the number of sector to being executed
TOTALSECTORCOUNT	equ 1023	; The number of total sector in VirtualOS

START:
	mov ax, cs
	mov ds, ax
	mov ax, 0xB800
	mov es, ax

; GENERATE CODES FOR EACH SECTOR
	%assign i	0	; assign variable i and initialise to 0
	%rep TOTALSECTORCOUNT	; repeat TOTALSECTORCOUNT
		%assign i	i + 1

		mov ax, 2

		mul word [ SECTORCOUNT ]
		mov si, ax

		mov byte [ es: si + (160 * 3) ], ('0' + ( i  % 10 ))
		add word [ SECTORCOUNT ], 1

		%if i == TOTALSECTORCOUNT
			jmp $
		%else
			jmp ( 0x1000 + i * 0x20 ): 0x0000
		%endif

		times ( 512 - ( $ - $$ ) % 512 ) db 0x00
	%endrep
