
DIRSDL=/users/prog1a/C/librairie/2011
CFLAGS=-c -g -O2 -I$(DIRSDL)/include/SDL -I$(DIRSDL)/include
LFLAGS=-L$(DIRSDL)/lib -lSDLmain -lSDL -lSDL_ttf -lSDL_image  -lSDL_sound -lSDL_phelma -lSDL_draw -lm

all : expression1 expression2 expression3

expression3 : expression3.o fonctions.o arbre.o traceexp.o simplifie.o
	gcc -o $@ $^  $(LFLAGS)

expression2 : expression2.o fonctions.o arbre.o traceexp.o
	gcc -o $@ $^  $(LFLAGS)

expression1 : expression1.o fonctions.o arbre.o traceexp.o
	gcc -o $@ $^  $(LFLAGS)

expression : expression.o fonctions.o arbre.o traceexp.o 
	gcc -o $@ $^  $(LFLAGS)

%.o : %.c
	gcc $(CFLAGS) $<

