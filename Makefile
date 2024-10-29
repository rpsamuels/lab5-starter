all: number_server

number_server: *.c
	gcc -std=c11 -Wall -Wno-unused-variable -fsanitize=address -g *.c -o number_server

clean: 
	rm number_server
