CFLAGS = -I ./include
LIB    = ./lib/fmod/libfmodex64.so ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: KangarooKO

KangarooKO: KangarooKO.c timing.c xwin.c ppm.c fmod.c log.c gameover.c startmenu.c bjarne.c minigame.c
	gcc $(CFLAGS) KangarooKO.c timing.c xwin.c ppm.c log.c fmod.c gameover.c startmenu.c bjarne.c minigame.c  -Wall -Wextra $(LFLAGS) -o KangarooKO

clean:
	rm -f KangarooKO
	rm -f *.o

