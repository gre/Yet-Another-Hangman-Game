DEBUG = -Wall -g
OPT = ${DEBUG}

all: clean pendu-txt pendu

pendu.o: pendu.c pendu.h
	gcc -c pendu.c -o pendu.o ${OPT}
	
dictionary.o: dictionary.c dictionary.h globals.h
	gcc -c dictionary.c -o dictionary.o ${OPT}
	
utils.o: utils.c utils.h
	gcc -c utils.c -o utils.o ${OPT}

userinterface_gui.o: userinterface_gui.c userinterface.h pendu.h
	gcc -c userinterface_gui.c -o userinterface_gui.o ${OPT}

userinterface_txt.o: userinterface_txt.c userinterface.h pendu.h
	gcc -c userinterface_txt.c -o userinterface_txt.o ${OPT}
	
pendu: pendu.o dictionary.o utils.o userinterface_gui.o main.c globals.h
	gcc main.c pendu.o dictionary.o utils.o userinterface_gui.o -lcurses -o pendu ${OPT}

pendu-txt: pendu.o dictionary.o utils.o userinterface_txt.o main.c globals.h
	gcc main.c pendu.o dictionary.o utils.o userinterface_txt.o -o pendu-txt ${OPT}

clean:
	rm -f *.o
