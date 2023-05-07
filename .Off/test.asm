
global main

section .data

string db      "%lf", 10
val    dq       101.0

section .text

; unset GTK_PATH  // use it in cmd line if edb does not work

extern printf

main:           mov r10, [val]
                push r10

                movlps xmm0, [rsp]
                cvttsd2si r10, xmm0
                cvttsd2si r10, xmm1
                cvttsd2si r10, xmm2
                cvttsd2si r10, xmm3

                movlps xmm0, [rsp]
                movlps xmm1, [rsp]
                movlps xmm2, [rsp]
                movlps xmm3, [rsp]
                
                mulsd xmm0, xmm1

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