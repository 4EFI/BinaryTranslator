
global main

section .data

string db      "%lf", 10
val1    dq       101.0
val2    dq       101.0

section .text

; unset GTK_PATH  // use it in cmd line if edb does not work

extern printf

main:           nop

                op:

                movlps xmm1, [rsp] 
                add    rsp, 8
                movlps xmm0, [rsp]
                add    rsp, 8
                movq   r10,  xmm0

                cmpsd xmm0, xmm1, 0
                cmpsd xmm0, xmm1, 1
                cmpsd xmm0, xmm1, 2

                
                cmp r10, 0x00
                jne 0x01

                je 0x10
                jne 0x123498982134 
                jbe 0x123498982134
                jb  0x123498982134

                ; cmp xmm0, 0

                mov r10, [val1]
                push r10

                cvttsd2si r10, xmm0
                cvttsd2si r10, xmm1
                cvttsd2si r10, xmm2
                cvttsd2si r10, xmm3

                movlps xmm0, [rsp]
                movlps xmm1, [rsp]
                movlps xmm2, [rsp]
                movlps xmm3, [rsp]
                
                divsd xmm0, xmm1

                movlps [rsp], xmm0

                push 100
                push 20         

                movlps xmm1, [rsp]
                add rsp, 8 
                movlps xmm0, [rsp] 

                addsd xmm0, xmm1
                movlps [rsp], xmm0

                mov rsi, 0xff00000000ff0000

                
                add rsp, 16
                sub rsp, 16

                call r10

                mov  r8, 0xff00000000ff0000
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

adb:    
    ret