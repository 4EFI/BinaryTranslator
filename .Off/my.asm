
call :main
ret

main:

jmp :a

push -120
push 20
mul
out

a:

push 100
push 100
jae :metka

metka:

ret