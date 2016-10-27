
all: test

test:insert.o main.o
	gcc -o test insert.o main.o

insert.o:insert.c
	gcc -g -c -o insert.o insert.c

main.o:main.c
	gcc -g -c -o main.o main.c
	
clean:
	rm -fr *.o test