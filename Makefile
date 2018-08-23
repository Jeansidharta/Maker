objectsFolder = objectFiles

exe: all
	./exe

all: createDir start

createDir:
	@if [ ! -d "$(objectsFolder)" ]; then mkdir $(objectsFolder);fi

start: $(objectsFolder)/main.o $(objectsFolder)/vector.o $(objectsFolder)/string.o $(objectsFolder)/list.o
	gcc -o exe $(objectsFolder)/main.o $(objectsFolder)/vector.o $(objectsFolder)/string.o $(objectsFolder)/list.o

$(objectsFolder)/main.o: main.c
	gcc -c main.c -o $(objectsFolder)/main.o

$(objectsFolder)/vector.o: vector.c
	gcc -c vector.c -o $(objectsFolder)/vector.o

$(objectsFolder)/string.o: string.c
	gcc -c string.c -o $(objectsFolder)/string.o

$(objectsFolder)/list.o: list.c
	gcc -c list.c -o $(objectsFolder)/list.o

zip: all
	zip main.zip main.c vector.c vector.h string.c string.h list.c list.h

valgrind: all
	valgrind ./exe