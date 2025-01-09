CC      = gcc
CFLAGS 	= -Wall -Wextra -pedantic -ggdb -std=c11 `pkg-config --cflags sdl2`
LIBS		=	`pkg-config --libs sdl2`

game: game.c
	$(CC) $(CFLAGS) -o game game.c $(LIBS) && ./game
