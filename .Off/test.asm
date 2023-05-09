
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

                movq xmm0, rax
                mov  r10, 0x0a
                movq xmm1, r10
                addsd xmm0, xmm1
                movq r10, xmm0

                push qword [rax]
                push qword [rcx]
                push qword [rdx]
                push qword [rbx]

                push rax
                push rcx
                push rdx
                push rbx

                add r10, rax
                add r10, rcx
                add r10, rdx
                add r10, rbx

                push r10
                push qword [r10]

                movlps xmm1, [val1] 
                sqrtsd xmm0, xmm1

adb:    
    ret