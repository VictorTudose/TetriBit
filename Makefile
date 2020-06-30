all:
	gcc -g tetris.c -o tema1 -Wall -lm
build: tetris.c
	gcc -g tetris.c -o tema1 -Wall -lm
run: build
	./tema1
clean: build
	rm tema1
