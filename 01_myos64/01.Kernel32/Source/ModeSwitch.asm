[BITS 32]

; Export function call
global kReadCPUID, kSwitchAndExecute64bitKernel

SECTION .text	; text section

; Return CPUID
;	PARAM: DWORD dwEAX, DWORD *pdwEAX, *pdwEBX, *pdwECX, *pdwEDX
kReadCPUID:
	push ebp		; Save caller's context
	mov ebp, esp
	push eax
	push ebx
	push ecx
	push edx
	push esi
;;;;;;;;;;;;;;;;;;;;;;;;;
; Execute CPUID command ;
;;;;;;;;;;;;;;;;;;;;;;;;;
	mov eax, dword [ ebp + 8 ]	; Set param 1 to eax register
	cpuid						; Call CPUID with param 1
								; CPUID instruction return CPU info to
								; various registers

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Store returned values to parameters ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; Set the value in eax to param2's address
	mov esi, dword [ ebp + 12 ]
	mov dword [ esi ], eax

	; Set the value in ebx to param3's address
	mov esi, dword [ ebp + 16 ]
	mov dword [ esi ], ebx

	; Set the value in ecx to param4's address
	mov esi, dword [ ebp + 20 ]
	mov dword [ esi ], ecx

	; Set the value in edx to param5's address
	mov esi, dword [ ebp + 24 ]
	mov dword [ esi ], edx

	; Restore caller's registers
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret

; Switch to IA-32e mode and execute 64-bit kernel
; PARAM: X
kSwitchAndExecute64bitKernel:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set CR4 register's PAE bit to 1 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
mov eax, cr4
or eax, 0x20
mov cr4, eax

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set CR3 register to PML4's table address (0x100000) ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
mov eax, 0x100000
mov cr3, eax


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set IA32_EFER.LME to 1 bit to activate IA-32e mode ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
mov eax, 0xC0000080	; Read IA32_EFER MSR register's value (mapped to 0xC0000080)
rdmsr				; Read MSR register

or eax, 0x0100		; Set LME bits
wrmsr				; Write MSR register

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Activate Cache and paging NW = 0 CD = 0 PG = 1 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
mov eax, cr0
or eax, 0xE0000000
xor eax, 0x60000000
mov cr0, eax

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Set cs segment selector to 0x08 and jump to 0x200000 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
jmp 0x08:0x200000

; Actually, not executed
jmp $
