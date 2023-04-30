
all : test.asm 
	nasm -f elf64 test.asm
	ld test.o -o test

my : my.asm
	../LngHandler/bin/asm my.asm -o my.code
