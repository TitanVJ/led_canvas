CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread

all: game

game: joyStick.o UDP.o main.o node
	$(CC_C) $(CFLAGS) -o game joyStick.o UDP.o main.o
	cp game $(HOME)/cmpt433/public/myApps/

%.o : %.c
	$(CC_C) -c $(CFLAGS) $<

node:
	mkdir -p $(HOME)/cmpt433/public/myApps/game-server-copy/
	cp -R Game_Dinosaur/* $(HOME)/cmpt433/public/myApps/game-server-copy/
	cd $(HOME)/cmpt433/public/myApps/game-server-copy/ && npm install

clean:
	rm -f *.o game
