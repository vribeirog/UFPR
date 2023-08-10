all: ballz

clean:
	-rm -f *.o

purge: clean
	-rm -f ballz

ballz: ballz.o game.o structs.o render.o
	gcc ballz.o game.o structs.o render.o -o ballz -Wall -std=c99 -g -lallegro_primitives -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf -lallegro -lm

ballz.o: ballz.c game.h structs.h render.h
	gcc -c ballz.c -o ballz.o -Wall -std=c99

game.o: game.c game.h structs.h render.h
	gcc -c game.c -o game.o -Wall

structs.o: structs.c structs.h
	gcc -c structs.c -o structs.o -Wall

render.o: render.c render.h structs.h
	gcc -c render.c -o render.o -Wall
