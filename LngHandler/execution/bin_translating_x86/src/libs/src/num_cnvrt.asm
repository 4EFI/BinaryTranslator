
section .text

extern putchar

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
								call putchar 	
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
								call putchar 	
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
				call putchar 	
				pop rax

				.Print:			mov  rsi, rsp 		; reverse print
								call putchar 	
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