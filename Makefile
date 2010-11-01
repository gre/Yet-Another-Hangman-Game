DEBUG =  -Wall -g

all: clean pendu

pendu.o: pendu.c pendu.h
	gcc -c pendu.c -o pendu.o ${DEBUG}
	
dictionary.o: dictionary.c dictionary.h globals.h
	gcc -c dictionary.c -o dictionary.o ${DEBUG}
	
utils.o: utils.c utils.h
	gcc -c utils.c -o utils.o ${DEBUG}
	
pendu: pendu.o dictionary.o utils.o main.c globals.h
	gcc main.c pendu.o dictionary.o utils.o -o pendu ${DEBUG}

clean:
	rm -f *.o
