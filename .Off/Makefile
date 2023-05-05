
BT = ./../LngHandler/bin/bin_translator
EX = ./../LngHandler/bin/cpu
AS = ./../LngHandler/bin/asm

all : test.asm 
	nasm -f elf64 test.asm
	ld test.o -o test

bin : my.asm
	$(AS) my.asm -o my.code
	$(EX) my.code
	$(BT) my.code
