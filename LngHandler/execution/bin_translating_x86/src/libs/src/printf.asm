
; BITS 64
; org 0x405000

section .text

str_num db  "%d", 0xA, 0x0

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

;------------------------------------------------
;   printf( str, ... )
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


;------------------------------------------------
;   num_cnvrt
;------------------------------------------------


;------------------------------------------------

section .text

;------------------------------------------------
;	print rax in bin
;------------------------------------------------
; entry:	rax = num
; exit:		none
; destroys:	none 
;------------------------------------------------

global print_bin

print_bin:	    ; proc				

				push rax 
				push rdx	

				xor  dx, dx		; i = 0

				.Next:			shr ax, 1		; AX /= 2			

								jc .One

								.Zero:			push '0'		; '0'
												jmp .End

								.One:			push '1'		; '1'

				.End:			inc dx				; i++

								cmp ax, 0			; if( ax > 0 )
								jg .Next

				.Print:			mov  rsi, rsp 		; reverse print
								call _putchar 	
								pop  rax						

								dec dx
								cmp dx, 0
								jne .Print 			; if( i != 0 )

				pop rdx
				pop rax			

				ret
				; endp

;------------------------------------------------
;	print rax in hex
;------------------------------------------------
; entry:	rax = num
; exit:		none
; destroys:	none  
;------------------------------------------------

global print_hex

print_hex:		; proc
							
				push rax 
				push rcx
				push rdx

				xor rdx, rdx	; i = 0

				.Next:			xor  cx, cx			; numShifts = 0			
	
								push rdx
								xor  rdx, rdx		; DX = 0 ( digit in hex )
								
								mov rcx, rax
								and rax, 15d
								mov rdx, rax
								mov rax, rcx
								shr rax, 4

								mov rcx, rax		; CX = AX
								
								cmp rdx, 10d		; if( DX >= 10 )
								jge .Sym

								.Digit:  		add  dx, '0'		; print( DX + '0' )
												xor  rax, rax
												mov  al, dl	
												jmp .Skip

								.Sym:			sub  dx, 10			; print( DX - 10 + 'A' )
												add  dx, 65d
												xor  rax, rax
												mov  al, dl

				.Skip:  		pop  rdx
								push rax			; for reverse print	

								mov ax, cx			; AX = CX

								inc dx				; i++

								cmp ax, 0			; if( ax > 0 )
								jg .Next

				.Print:			mov  rsi, rsp 		; reverse print
								call _putchar 	
								pop  rax						

								dec dx
								cmp dx, 0
								jne .Print 			; if( i != 0 )

				pop rdx
				pop rcx
				pop rax

				ret
				; endp

;------------------------------------------------
;	print rax in a number system ( <= 10 )
;------------------------------------------------
; entry:	rax = num
;			bx = system degree
; exit:		none
; destroys:	none
;------------------------------------------------

global print_num	

print_num:		; proc
				
				push rax 
				push rbx
				push rcx 
				push rdx
				push r8

				push rax
				mov  rcx, 1
				shl  rcx, 16
				and  rax, rcx
				mov  r8, rax
				pop  rax
				cmp  r8, 0
				je	.NoMinus

				mov rcx, 1
				shl rcx, 16
				sub rax, rcx
				neg rax

				.NoMinus:

				xor rdx, rdx			; i = 0
				mov rcx, rbx			; CX = degree

				.Next:			push rdx				

								xor rdx, rdx		; DX = 0

								div  cx				; AX /= degree

								push rax				

								xor rax, rax
								add dl, 48d		; print( DL + '0' )
								mov al, dl
								mov rbx, rax			

								pop rax	
								pop rdx

								push rbx

								inc dx

								cmp ax, 0			; if( AX > 0 )
								jne .Next

				cmp r8, 0
				je .Print

				push '-'
				mov  rsi, rsp 			; print '-'
				call _putchar 	
				pop rax

				.Print:			mov  rsi, rsp 		; reverse print
								call _putchar 	
								pop  rax						

								dec dx
								cmp dx, 0
								jne .Print 			; if( i != 0 )
				
				pop r8
				pop rdx 
				pop rcx 
				pop rbx
				pop rax		

				ret 
				; endp

;------------------------------------------------

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
                
