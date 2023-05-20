
include SOURCES

#compiling & linking

OBJ_TMP = $(subst .asm,.o,       $(SRC))
OBJ     = $(subst src,./, $(OBJ_TMP))

#compiling
                                             
link : $(OBJ) 
	echo 

%.o : src/%.asm	
	mkdir -p $(@D)
#nasm -f bin -o printf $^
	nasm -f elf64 -o printf.o $^

clear :
	rm   -f ./*.o    