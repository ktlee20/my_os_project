[BITS 64]			; the below codes are 64-bit ones

SECTION	.text		; define text section

; Import functions from external files
extern kcommon_exception_handler, kcommon_interrupt_handler, kkeyboard_handler

; Export functions in this file
; For exception handling
global kISRDivideError, kISRDebug, kISRNMI, kISRBreakPoint, kISROverflow
global kISRBoundRangeExceeded, kISRInvalidOpcode, kISRDeviceNotAvailable, kISRDoubleFault,
global kISRCoprocessorSegmentOverrun, kISRInvalidTSS, kISRSegmentNotPresent
global kISRStackSegmentFault, kISRGeneralProtection, kISRPageFault, kISR15
global kISRFPUError, kISRAlignmentCheck, kISRMachineCheck, kISRSIMDError, kISRETCException

; For interrupt handling
global kISRTimer, kISRKeyboard, kISRSlavePIC, kISRSerial2, kISRSerial1, kISRParallel2
global kISRFloppy, kISRParallel1, kISRRTC, kISRReserved, kISRNotUsed1, kISRNotUsed2
global kISRMouse, kISRCoprocessor, kISRHDD1, kISRHDD2, kISRETCInterrupt

; Macroes for context switch
; Save current context
%macro KSAVECONTEXT 0
	push rbp
	mov rbp, rsp
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov ax, ds ; Since DS and ES segment selectors can't be directly inserted,
	push rax   ; rax register is used.
	mov ax, es
	push rax
	push fs
	push gs

	; Switching segment selectors
	mov ax, 0x10 ; Set kernel segment descriptor(0x10) to ax register
	mov ds, ax	 ; change ds, es, gs, fs to kernel segment descriptor
	mov es, ax
	mov gs, ax
	mov fs, ax
%endmacro		; End this macro


; Restore saved context
%macro KLOADCONTEXT 0
	; Restore GS segment selector to RBP register from stack
	pop gs
	pop fs
	pop rax		; Since ES and DS segment selectors can' be directly popped,
	mov es, ax	; rax register is used.
	pop rax
	mov ds, ax

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop rbp
%endmacro

;;;;;;;;;;;;;;;;;;;;;;
; Exception handlers ;
;;;;;;;;;;;;;;;;;;;;;;

; #0 Divide error
kISRDivideError:
	KSAVECONTEXT
	; Set exception number and call handler
	mov rdi, 0
	call kcommon_exception_handler

	KLOADCONTEXT
	iretq

; #1, Debug ISR
kISRDebug:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 1
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #2, NMI ISR
kISRNMI:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 2
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #3, BreakPoint ISR
kISRBreakPoint:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 3
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #4, Overflow ISR
kISROverflow:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 4
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #5, Bound Range Exceeded ISR
kISRBoundRangeExceeded:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 5
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #6, Invalid Opcode ISR
kISRInvalidOpcode:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 6
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #7, Device Not Available ISR
kISRDeviceNotAvailable:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 7
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #8, Double Fault ISR
kISRDoubleFault:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호와 에러 코드를 삽입하고 핸들러 호출
    mov rdi, 8
    mov rsi, qword [ rbp + 8 ]
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    add rsp, 8      ; 에러 코드를 스택에서 제거
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #9, Coprocessor Segment Overrun ISR
kISRCoprocessorSegmentOverrun:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 9
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #10, Invalid TSS ISR
kISRInvalidTSS:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호와 에러 코드를 삽입하고 핸들러 호출
    mov rdi, 10
    mov rsi, qword [ rbp + 8 ]
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    add rsp, 8      ; 에러 코드를 스택에서 제거
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #11, Segment Not Present ISR
kISRSegmentNotPresent:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호와 에러 코드를 삽입하고 핸들러 호출
    mov rdi, 11
    mov rsi, qword [ rbp + 8 ]
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    add rsp, 8      ; 에러 코드를 스택에서 제거
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #12, Stack Segment Fault ISR
kISRStackSegmentFault:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호와 에러 코드를 삽입하고 핸들러 호출
    mov rdi, 12
    mov rsi, qword [ rbp + 8 ]
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    add rsp, 8      ; 에러 코드를 스택에서 제거
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #13, General Protection ISR
kISRGeneralProtection:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호와 에러 코드를 삽입하고 핸들러 호출
    mov rdi, 13
    mov rsi, qword [ rbp + 8 ]
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    add rsp, 8      ; 에러 코드를 스택에서 제거
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #14, Page Fault ISR
kISRPageFault:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호와 에러 코드를 삽입하고 핸들러 호출
    mov rdi, 14
    mov rsi, qword [ rbp + 8 ]
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    add rsp, 8      ; 에러 코드를 스택에서 제거
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #15, Reserved ISR
kISR15:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 15
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #16, FPU Error ISR
kISRFPUError:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 16
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #17, Alignment Check ISR
kISRAlignmentCheck:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 17
    mov rsi, qword [ rbp + 8 ]
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    add rsp, 8      ; 에러 코드를 스택에서 제거
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #18, Machine Check ISR
kISRMachineCheck:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 18
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #19, SIMD Floating Point Exception ISR
kISRSIMDError:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 19
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #20~#31, Reserved ISR
kISRETCException:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 예외 번호를 삽입하고 핸들러 호출
    mov rdi, 20
    call kcommon_exception_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #32, 타이머 ISR
kISRTimer:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 32
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #33, 키보드 ISR
kISRKeyboard:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 33
    call kkeyboard_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #34, 슬레이브 PIC ISR
kISRSlavePIC:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 34
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #35, 시리얼 포트 2 ISR
kISRSerial2:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 35
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #36, 시리얼 포트 1 ISR
kISRSerial1:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 36
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #37, 패러렐 포트 2 ISR
kISRParallel2:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 37
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #38, 플로피 디스크 컨트롤러 ISR
kISRFloppy:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 38
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #39, 패러렐 포트 1 ISR
kISRParallel1:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 39
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #40, RTC ISR
kISRRTC:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 40
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #41, 예약된 인터럽트의 ISR
kISRReserved:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 41
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #42, 사용 안함
kISRNotUsed1:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 42
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #43, 사용 안함
kISRNotUsed2:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 43
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #44, 마우스 ISR
kISRMouse:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 44
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #45, 코프로세서 ISR
kISRCoprocessor:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 45
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #46, 하드 디스크 1 ISR
kISRHDD1:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 46
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #47, 하드 디스크 2 ISR
kISRHDD2:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 47
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원

; #48 이외의 모든 인터럽트에 대한 ISR
kISRETCInterrupt:
    KSAVECONTEXT    ; 콘텍스트를 저장한 뒤 셀렉터를 커널 데이터 디스크립터로 교체

    ; 핸들러에 인터럽트 번호를 삽입하고 핸들러 호출
    mov rdi, 48
    call kcommon_interrupt_handler

    KLOADCONTEXT    ; 콘텍스트를 복원
    iretq           ; 인터럽트 처리를 완료하고 이전에 수행하던 코드로 복원
