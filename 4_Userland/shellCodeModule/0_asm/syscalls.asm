GLOBAL sys_time
GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_clear
GLOBAL sys_set_font_scale
GLOBAL sys_draw_rectangle
GLOBAL sys_tick
GLOBAL sys_sleep
GLOBAL sys_reset_cursor
GLOBAL sys_get_regs
GLOBAL sys_beep
GLOBAL sys_draw_pixel

; Memory Manager syscalls
GLOBAL sys_my_malloc
GLOBAL sys_my_free
GLOBAL sys_mem_dump

; Scheduler syscalls
GLOBAL sys_new_process
GLOBAL sys_exit
GLOBAL sys_get_pid
GLOBAL sys_process_status
GLOBAL sys_kill_process
GLOBAL sys_set_priority
GLOBAL sys_block_process
GLOBAL sys_unblock_process
GLOBAL sys_yield
GLOBAL sys_waitpid

section .text

%macro pushState 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
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
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro


%macro pushStateMinusRax 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi

%endmacro

%macro popStateMinusRax 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
%endmacro


%macro sys_interrupt 1
        mov rax, %1
        int 80h
%endmacro



%macro sys_handler 1
    push rbp
    mov rbp, rsp
    mov rax, %1
    int 80h
    mov rsp, rbp
    pop rbp
    ret
%endmacro

sys_sleep:
    sys_handler 0

sys_time:
    sys_handler 1

sys_set_font_scale:
    sys_handler 2

sys_read:
    sys_handler 3

sys_write:
    sys_handler 4

sys_clear:
    sys_handler 5

sys_draw_rectangle:
    sys_handler 6

sys_tick:
    sys_handler 7

sys_reset_cursor:
    sys_handler 8

sys_get_regs:
    sys_handler 9
    
sys_beep:
    sys_handler 10

sys_draw_pixel:
    sys_handler 11

; Memory Manager syscalls
sys_my_malloc:
    sys_handler 12
sys_my_free:
    sys_handler 13
sys_mem_dump:
    sys_handler 14

; Scheduler syscalls
sys_new_process:
    sys_handler 15

sys_exit:
    sys_handler 16

sys_kill_process:
    sys_handler 17

sys_get_pid:
    sys_handler 18

sys_process_status:
    sys_handler 19

sys_set_priority:
    sys_handler 20

sys_block_process:
    sys_handler 21

sys_unblock_process:
    sys_handler 22

sys_yield:
    sys_handler 23

sys_waitpid:
    sys_handler 24

