build:
	gcc -Wall -std=c89 ./src/main.c -o bin/cprog

run:	
	./bin/cprog

clean:
	rm -rf bin/cprog