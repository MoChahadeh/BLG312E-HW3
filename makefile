all: main.o
	make main.o
	./main.o
	make clean

main.o: main.c
	gcc -o main.o main.c

clean:
	rm -f main.o main