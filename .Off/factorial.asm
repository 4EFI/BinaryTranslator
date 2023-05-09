
Start:
	push 5
	call :Factorial
	out

	hlt

Factorial:
	pop rax

	push 1
	pop rbx

	call :DoFactorial
	push rbx

	ret

DoFactorial:  	
	push 0
	push rax
	je :endDoFactorial

	push rbx
    push rax
	mul
	pop rbx

	push rax + -1 ; Hehe
	pop rax

	call :DoFactorial	

	endDoFactorial:
	ret

         