build:
	gcc -Wall \
	-g \
	-Wfatal-errors \
	-std=c99 \
	./src/main.c \
	./src/display.c \
	-lSDL2 \
	-o bin/cprog

run:	
	./bin/cprog

clean:
	rm -rf bin/cprog
