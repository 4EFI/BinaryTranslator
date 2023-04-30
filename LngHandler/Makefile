
include SOURCES

#compiling & linking

OBJ_TMP = $(subst .cpp,.o,       $(SRC))
OBJ     = $(subst src,src/build, $(OBJ_TMP))

#compiling

ifdef MAIN_MAKE

all : $(LIBS) $(OBJ) 
	make -C ./front_end	    		-f    ../Makefile
	make -C ./back_end      		-f    ../Makefile
	make -C ./compiling     		-f    ../Makefile
	make -C ./execution/ASM 		-f ../../Makefile
	make -C ./execution/CPU 	  	-f ../../Makefile
	make -C ./bin_translating_x86 	-f    ../Makefile

else

link : $(LIBS) $(OBJ) 
	$(CC) $(IFLAGS) $(CFLAGS) $^ -o $(OUTPUT_FILE_NAME) 

endif


src/build/%.o : src/%.cpp  	
	echo  $(@D)
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@	

ifdef MAIN_MAKE

clear :
	rm   -f ./src/build/*.o
	make -C ./front_end     		-f    ../Makefile clear 
	make -C ./back_end      		-f    ../Makefile clear
	make -C ./compiling     		-f    ../Makefile clear
	make -C ./execution/ASM 		-f ../../Makefile clear
	make -C ./execution/CPU 		-f ../../Makefile clear
	make -C ./bin_translating_x86 	-f    ../Makefile clear

else

clear :
	rm -f ./src/build/*.o	

endif

