call :Mein
hlt
Reshit'Kvadratku:
push rax + 0
pop rbx ; set "a" pos

pop [ rbx ]

push rax + 1
pop rbx ; set "b" pos

pop [ rbx ]

push rax + 2
pop rbx ; set "ts" pos

pop [ rbx ]

push rax + 0
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
push 0
is_ee
push 0
je :endif001

push rax + 2
pop rbx ; set "ts" pos

push [ rbx ] ; push "ts"
push rax + 1
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
push 3 + rax
pop rax ; Shifting top the var register
call :Reshit'Lineinoe
push -3 + rax
pop rax ; Shifting down the var register
ret

endif001:

push rax + 1
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
push 2
pow
push 4
push rax + 0
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
mul
push rax + 2
pop rbx ; set "ts" pos

push [ rbx ] ; push "ts"
mul
sub
push rax + 3
pop rbx ; set "D" pos

pop [ rbx ] ; set "D"

push rax + 3
pop rbx ; set "D" pos

push [ rbx ] ; push "D"
push 0
is_gt
push 0
je :endif002

push 0
push rax + 1
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
sub
push rax + 3
pop rbx ; set "D" pos

push [ rbx ] ; push "D"
sqrt
add
push 2
push rax + 0
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
mul
div
out
push 0
push rax + 1
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
sub
push rax + 3
pop rbx ; set "D" pos

push [ rbx ] ; push "D"
sqrt
sub
push 2
push rax + 0
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
mul
div
out
push 2
ret

endif002:

push rax + 3
pop rbx ; set "D" pos

push [ rbx ] ; push "D"
push 0
is_ee
push 0
je :endif003

push 0
push rax + 1
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
sub
push 2
push rax + 0
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
mul
div
out
push 1
ret

endif003:

push 0
ret
ret
endReshit'Kvadratku:
Reshit'Lineinoe:
push rax + 0
pop rbx ; set "a" pos

pop [ rbx ]

push rax + 1
pop rbx ; set "b" pos

pop [ rbx ]

push rax + 0
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
push 0
is_ne
push 0
je :endif004

push 0
push rax + 1
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
sub
push rax + 0
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
div
out
push 1
ret

endif004:

push rax + 1
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
push 0
is_ee
push 0
je :endif005

push 99999
out
push 99999
ret

endif005:

push 0
ret
ret
endReshit'Lineinoe:
Faktorial:
push rax + 0
pop rbx ; set "n" pos

pop [ rbx ]

push rax + 0
pop rbx ; set "n" pos

push [ rbx ] ; push "n"
push 1
is_be
push 0
je :endif006

push 1
ret

endif006:

push rax + 0
pop rbx ; set "n" pos

push [ rbx ] ; push "n"
push 1
sub
push 1 + rax
pop rax ; Shifting top the var register
call :Faktorial
push -1 + rax
pop rax ; Shifting down the var register
push rax + 0
pop rbx ; set "n" pos

push [ rbx ] ; push "n"
mul
ret
ret
endFaktorial:
Mein:
push 0
push rax + 0
pop rbx ; set "f" pos

pop [ rbx ] ; set "f"

in
push rax + 0
pop rbx ; set "f" pos

pop [ rbx ]
push rax + 0
pop rbx ; set "f" pos

push [ rbx ] ; push "f"
push 1 + rax
pop rax ; Shifting top the var register
call :Faktorial
push -1 + rax
pop rax ; Shifting down the var register
out
push 0
push rax + 1
pop rbx ; set "a" pos

pop [ rbx ] ; set "a"

push 0
push rax + 2
pop rbx ; set "b" pos

pop [ rbx ] ; set "b"

push 0
push rax + 3
pop rbx ; set "ts" pos

pop [ rbx ] ; set "ts"

in
push rax + 1
pop rbx ; set "a" pos

pop [ rbx ]
in
push rax + 2
pop rbx ; set "b" pos

pop [ rbx ]
in
push rax + 3
pop rbx ; set "ts" pos

pop [ rbx ]
push rax + 3
pop rbx ; set "ts" pos

push [ rbx ] ; push "ts"
push rax + 2
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
push rax + 1
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
push 4 + rax
pop rax ; Shifting top the var register
call :Reshit'Kvadratku
push -4 + rax
pop rax ; Shifting down the var register
push 0
ret
ret
endMein:
