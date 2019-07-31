all: assembler

assembler:	Main.o Assembler.o
	gcc -g -o Assembler Main.o Assembler.o
	./Assembler $(ARGS)

Assembler.o: Assembler.c
	gcc -c Assembler.c

Main.o: Main.c
	gcc -c Main.c

clean:
	rm -rf *.o
	rm -rf Assembler

run: Assembler
	./Assembler ${ARGS}
