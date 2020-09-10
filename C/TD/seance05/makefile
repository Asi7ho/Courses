#$@     Le nom de la cible
#$<     Le nom de la première dépendance
#$^     La liste des dépendances
#$*     Le nom du fichier sans suffixe

DIRSDL=/users/prog1a/C/librairie/2011

CFLAGS=-g -I$(DIRSDL)/include -I$(DIRSDL)/include/SDL -I/usr/local/include -I/usr/local/include/SDL -I/usr/include/SDL
LDFLAGS=-L$(DIRSDL)/lib -lSDL -lSDL_ttf -lSDL_image  -lSDL_sound -lSDL_phelma -lSDL_draw -lX11


SRC = prog.c fonction.c
OBJS = $(SRC:.c=.o)


prog : $(OBJS)
	gcc -o $@ $^ $(LDFLAGS)

%.o: %.c
	gcc -c $< $(CFLAGS)
        
clean :
	rm -f *.o test? *~

