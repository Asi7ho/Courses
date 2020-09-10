DIRSDL=/users/prog1a/C/librairie/2011

CFLAGS=-g -I$(DIRSDL)/include -I$(DIRSDL)/include/SDL -I/usr/local/include -I/usr/local/include/SDL -I/usr/include/SDL

LDFLAGS= -L$(DIRSDL)/lib -L/usr/local/lib -lSDL -lSDL_ttf -lSDL_image -lSDL_draw -lSDL_phelma -lSDL_sound -lX11 -lm

corde : tp3b.o
	gcc -o $@ $^ $(LDFLAGS)

corde.o : tp3b.c
	gcc -c $< $(CFLAGS)
