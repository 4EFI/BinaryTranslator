
section .text
global _start

; unset GTK_PATH  // use it in cmd line if edb does not work

_start:         mov  r8, 0xff00000000ff0000
                push r8

                push 0xff00000000ff0000

                push rax
                push rcx
                push rdx
                push rbx
                push rsi 
                push rdi
                push r8
                push r9
                push r10
                push r11
                push r12

                add rax, 100
                add rcx, rax
    
                mov rax, 0x3c
                xor rdi, rdi
                syscall