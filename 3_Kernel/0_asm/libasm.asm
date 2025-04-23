GLOBAL cpuVendor, rtc, getPressedKey, _updateRegisters, _getRegisters
GLOBAL outb
GLOBAL inb

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

rtc:
    push rbp
    mov rbp, rsp

    mov al, dil
    out 70h, al
    in al, 71h

    mov rsp, rbp
    pop rbp
    ret

getPressedKey:
	push rbp
	mov rbp, rsp

	in al, 60h
	leave
	ret

_updateRegisters:
    mov [regs], rax
    mov [regs + 1*8], rbx
    mov [regs + 2*8], rcx
    mov [regs + 3*8], rdx
    mov [regs + 4*8], rsi
    mov [regs + 5*8], rdi
    mov [regs + 6*8], rbp
    mov [regs + 7*8], r8
    mov [regs + 8*8], r9
    mov [regs + 9*8], r10
    mov [regs + 10*8], r11
    mov [regs + 11*8], r12
    mov [regs + 12*8], r13
    mov [regs + 13*8], r14
    mov [regs + 14*8], r15
    mov rax, [rsp]
    mov [regs + 15*8], rax ; instruction pointer
    mov rax, rsp
    mov [regs + 16*8], rax
    ret

_getRegisters:
    mov rax, regs
    ret


inb:
	push rbp
    mov rbp, rsp

	mov rdx, rdi
	in al, dx

	mov rsp, rbp
	pop rbp
	ret

outb:
	push rbp
    mov rbp, rsp

	mov rax, rsi
	mov rdx, rdi
	out dx, al

	mov rsp, rbp
	pop rbp
	ret

section .bss
    regs dq 17
