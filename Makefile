all:
	mkdir -p bin
	gcc -std=c17 -o bin/main src/Field.c src/main.c src/Vector2.c src/Vector2Queue.c -lncurses

run:
	./bin/main

clean:
	rm -f bin/*
