CFLAGS = -I ./include
LIB    = ./lib/fmod/libfmodex64.so ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: KangarooKO

KangarooKO: KangarooKO.c timing.c xwin.c ppm.c fmod.c log.c Jose.c Gabe.c Iggy.c Bjarne.c Angel.c
	gcc $(CFLAGS) KangarooKO.c timing.c xwin.c ppm.c log.c fmod.c Jose.c Gabe.c Iggy.c Bjarne.c Angel.c -Wall -Wextra $(LFLAGS) -o KangarooKO

clean:
	rm -f KangarooKO
	rm -f *.o

