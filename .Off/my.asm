
Start:
	push 5
    pop  rax  
	call :Factorial
	push rbx
    out

	ret

Factorial:
	push 1
	pop rbx

	call :DoFactorial

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

         