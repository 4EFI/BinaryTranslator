call :Mein
hlt
Mein:
push 5
push rax + 0
pop rbx ; set "f" pos

pop [ rbx ] ; set "f"

push rax + 0
pop rbx ; set "f" pos

push [ rbx ] ; push "f"
out
push 6
push rax + 8
pop rbx ; set "a" pos

pop [ rbx ] ; set "a"

push -2
push rax + 16
pop rbx ; set "b" pos

pop [ rbx ] ; set "b"

push 2
push rax + 24
pop rbx ; set "ts" pos

pop [ rbx ] ; set "ts"

push 3
push rax + 32
pop rbx ; set "r" pos

pop [ rbx ] ; set "r"

push rax + 8
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
out
push rax + 16
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
out
push rax + 24
pop rbx ; set "ts" pos

push [ rbx ] ; push "ts"
out
push rax + 32
pop rbx ; set "r" pos

push [ rbx ] ; push "r"
out
push 0
ret
endMein:
