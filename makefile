build:
	gcc -Wall \
	-g \
	-Wfatal-errors \
	-std=c89 \
	./src/main.c \
	-I"C:\libsdl\include" \
	-L"C:\libsdl\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-o bin/cprog

run:	
	./bin/cprog

clean:
	rm -rf bin/cprog