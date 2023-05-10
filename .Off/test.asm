
global main

section .data

string db      "%lf", 10
val1    dq       101.0
val2    dq       101.0

section .text

; unset GTK_PATH  // use it in cmd line if edb does not work

extern printf

main:           nop

                xor rax, rax
                xor rcx, rcx
                xor rdx, rdx
                xor rbx, rbx

                op:

                push qword [rbp]

                mov rbp, r10 

                mov qword [rbp], r10

                add rbp, 8
                sub rbp, 8

                pop qword [r10]

                mov rbp, 0x00fff00ff00f0f0f0
                push rbp

                pop qword [rax]
                pop qword [rcx]
                pop qword [rdx]
                pop qword [rbx]

                sqrtsd xmm0, xmm1

                push qword [r10]

                movq xmm1, r10
                movq xmm0, r10

                pop qword [r10]

                movq xmm0, rax
                movq xmm0, rcx
                movq xmm0, rdx
                movq xmm0, rbx

                movq xmm1, rax
                movq xmm1, rcx
                movq xmm1, rdx
                movq xmm1, rbx

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

                pop rax
                pop rcx
                pop rdx
                pop rbx

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