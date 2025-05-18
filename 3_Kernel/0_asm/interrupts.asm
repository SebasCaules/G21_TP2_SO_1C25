
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL getSnap
GLOBAL setup_stack_frame
GLOBAL timer_tick

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN sysCallHandler
EXTERN getStackBase

section .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
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
%endmacro

%macro popState 0
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
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro popStateNoRAX 0
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
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro snapshot 0
    mov [regs + (8 * 0)], rax
    mov [regs + (8 * 1)], rbx
    mov [regs + (8 * 2)], rcx
    mov [regs + (8 * 3)], rdx
    mov [regs + (8 * 4)], rsi
    mov [regs + (8 * 5)], rdi
    mov [regs + (8 * 6)], rbp
    mov [regs + (8 * 7)], r8
    mov [regs + (8 * 8)], r9
    mov [regs + (8 * 9)], r10
    mov [regs + (8 * 10)], r11
    mov [regs + (8 * 11)], r12
    mov [regs + (8 * 12)], r13
    mov [regs + (8 * 13)], r14
    mov [regs + (8 * 14)], r15

    mov rax, [rsp + (8 * 3)] ;rsp
    mov [regs + (8 * 15)], rax

    mov rax, [rsp + (8 * 0)] ;rip
    mov [regs + (8 * 16)], rax

	; estos no se 
    mov rax, [rsp + (8 * 2)] ;rflags
    mov [regs + (8 * 17)], rax

    mov rax, [rsp + (8 * 1)] ;cs
    mov [regs + (8 * 18)], rax

    mov rax, [rsp + (8 * 4)] ;ss
    mov [regs + (8 * 19)], rax
%endmacro



%macro exceptionHandler 1

    snapshot

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	call getStackBase

	mov [rsp+24], rax

	mov rax, userland
	mov [rsp], rax

	iretq
%endmacro

getSnap:
	mov rax, regs
	ret

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

timer_tick:
	int 20h
	ret

;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Sys Calls
_irq80Handler:
	pushState
	mov rdi, rsp
	call sysCallHandler
	popStateNoRAX
	add rsp, 8
	iretq


setup_stack_frame:
	mov r8, rsp
	mov r9, rbp
	mov rsp, rdx
	mov rbp, rdx
	push 0x0
	push rdx
	push 0x202
	push 0x8
	push rdi
	mov rdi, rsi
	mov rsi, rcx
	pushState
	mov rax, rsp
	mov rsp, r8
	mov rbp, r9
	ret


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Opcode Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret


section .rodata
    userland equ 0x400000

section .bss
	aux resq 1
	regs resq 20