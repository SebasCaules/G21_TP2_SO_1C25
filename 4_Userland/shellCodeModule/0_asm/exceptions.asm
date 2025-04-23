GLOBAL divZero
GLOBAL opCode


section .text

divZero:
    push rbp
    mov rbp, rsp

    mov rdx, 1
    mov rax, 0
    div rax

    mov rsp, rbp
    pop rbp
    ret

opCode:
    ud2
    ret
    ; push rbp
    ; mov rbp, rsp

    ; jmp 0

    ; mov rsp, rbp
    ; pop rbp
    ; ret