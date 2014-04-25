CFLAGS = -I ./include
LIB    = ./lib/fmod/libfmodex64.so ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: rainforest

rainforest: rainforest.c ppm.c fmod.c log.c
	gcc $(CFLAGS) rainforest.c ppm.c log.c fmod.c -Wall -Wextra $(LFLAGS) -o rainforest

clean:
	rm -f rainforest
	rm -f *.o

