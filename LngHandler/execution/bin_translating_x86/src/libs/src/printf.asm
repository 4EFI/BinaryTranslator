
section .text

extern printf

extern print_bin, print_hex, print_num

;------------------------------------------------
;   printf( str, ... )
;------------------------------------------------

section .rodata

JumpTable:  times '%'               dq None         ; [ '\0'; '%' ) 
                                    dq Percent      ; '%%'
            times ( 'b'-'%' - 1 )   dq None         ; ( '%'; 'b' ) 
                                    dq Bin          ; '%b'
                                    dq Char         ; '%c'
                                    dq Decimal      ; '%d'
            times ( 'o'-'d' - 1 )   dq None         ; ( 'd'; 'o' )                                     
                                    dq Oct          ; '%o'
            times ( 's'-'o' - 1 )   dq None         ; ( 'o'; 's' )
                                    dq String       ; '%s'   
            times ( 'x'-'s' - 1 )   dq None         ; ( 's'; 'x' )
                                    dq Hex          ; '%x'
            times ( 255 - 'x' )     dq None

section .text

%macro	        GetArgAddr 0
                mov  rsi, rbp       ; rsi = rbp + 8 + 8*args_counter
                add  rsi, 8
                shl  rcx, 3
                add  rsi, rcx
                shr  rcx, 3
                inc  rcx            ; args_counter++
%endmacro

None:           push rsi
                push '%'
                mov  rsi, rsp 
                call _putchar
                pop  rsi
                pop  rsi
                ret

Percent:        call _putchar
                inc  rsi
                ret

Char:           push rsi
                GetArgAddr
                call _putchar
                pop  rsi
                inc  rsi
                ret

Bin:            push rsi
                GetArgAddr 
                mov  rax, [rsi]
                call print_bin
                pop  rsi
                inc  rsi
                ret

Decimal:        push rsi
                GetArgAddr 
                mov  rax, [rsi]
                mov  rbx, 10d
                call print_num
                pop  rsi
                inc  rsi
                ret

Oct:            push rsi
                GetArgAddr 
                mov  rax, [rsi]
                mov  rbx, 8d
                call print_num
                pop  rsi
                inc  rsi
                ret

String:         push rsi
                GetArgAddr 
                mov  rsi, [rsi] 
                call _puts
                pop  rsi
                inc  rsi
                ret

Hex:            push rsi
                GetArgAddr 
                mov  rax, [rsi]
                call print_hex
                pop  rsi
                inc  rsi
                ret

;------------------------------------------------

global _printf

_printf:      ; proc        

                push rbp
                mov  rbp, rsp
                add  rbp, 16d               ; + ret_addr + push_rbp 

                push rsi
                mov  rsi, [rbp]             ; rsi = str addr

                push rax
                push rbx
                push rcx                    

                xor rcx, rcx                ; args_counter = 0

                .Next:          cmp byte [rsi], 0       ; if( curr_sym == '\0' )
                                je .End

                                cmp byte [rsi], '%'     ; if( curr_sym == '%' )
                                jne .NotPercent

                                inc  rsi
                                xor  rbx, rbx
                                mov  bl, [rsi]
                                push .Next
                                jmp [JumpTable + 8*rbx] ; go to JumpTable

                                .NotPercent:

                                call _putchar
                                inc  rsi

                                jmp .Next       
                .End:

                pop rcx
                pop rbx
                pop rax
                pop rsi
                pop rbp

                ret
                ; endp

global c_printf

c_printf        ; proc
                
                ;                 sp 
                ; Stack now: | ret addr | other pushs |
                
                mov [rsp - 6*8], rbx        ; save rbx ( keep mem for 6 pushs )
                mov  rbx, [rsp]             ; rbx = ret addr 
                add  rsp, 8

                ;                                           sp
                ; Stack now: | rbx | ... for 6 pushs | other pushs |
                
                push r9
                push r8
                push rcx
                push rdx
                push rsi
                push rdi

                ;                       sp
                ; Stack now: | rbx | 6 pushs | other pushs |
                
                call _printf

                add  rsp, 6*8

                call printf

                push rbx                    ; push ret addr
                mov  rbx, [rsp-6*8]         ; load rbx

                ;                 sp
                ; Stack now: | ret addr | other pushs |

                ret
                ; endp

;------------------------------------------------
;   _putchar( char* ch_ptr )
;------------------------------------------------
; entry:    rsi = ch_ptr
; exit:     none
; destroys: none
;------------------------------------------------

global _putchar

_putchar:        ; proc

                push rax
                push rdi
                push rdx
                push rcx
                push r11

                mov rax, 1      ; write( edi = stdout, rsi = ch_ptr, rdx = 1 ( len ) )
                mov rdi, 1      
                mov rdx, 1      
                syscall         

                pop r11
                pop rcx
                pop rdx
                pop rdi
                pop rax

                ret
                ; endp

;------------------------------------------------
;   _puts( char* str_ptr )
;------------------------------------------------
; entry:    rsi = str_ptr
; exit:     none
; expects:  end sym = '\0'
; destroys: none
;------------------------------------------------

global _puts

_puts:           ; proc

                push rsi

                .Next:          cmp byte [rsi], 0   ; if( curr_sym == '\0' )
                                je .End

                                call _putchar
                                inc  rsi            ; str_ptr++

                                jmp .Next
                .End:

                pop rsi

                ret
                ; endp

;------------------------------------------------
                
