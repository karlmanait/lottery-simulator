all: build

build:
	gcc -c src/main.c src/config.c src/display.c
	gcc main.o config.o display.o -o lottery_sim -lncursesw -lpthread
	rm *.o
