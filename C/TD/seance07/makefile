DIRSDL=/users/prog1a/C/librairie/2011
CFLAGS=-g -I$(DIRSDL)/include -I$(DIRSDL)/include/SDL -I/usr/local/include -I/usr/local/include/SDL -I/usr/include/SDL
LDFLAGS=-L$(DIRSDL)/lib -lSDL -lSDL_ttf -lSDL_image  -lSDL_sound -lSDL_phelma -lSDL_draw -lX11 -lm

dessin: dessin.o
	gcc -o $@ $^  $(LDFLAGS)

%.o: %.c
	gcc -c $(CFLAGS) $< 
