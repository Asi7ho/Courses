#$@  	Le nom de la cible
#$< 	Le nom de la première dépendance
#$^ 	La liste des dépendances
#$* 	Le nom du fichier sans suffixe

DIRSDL=/users/prog1a/C/librairie/2011
CFLAGS=-g -I$(DIRSDL)/include -I$(DIRSDL)/include/SDL -I/usr/local/include -I/usr/local/include/SDL -I/usr/include/SDL
LDFLAGS=-L$(DIRSDL)/lib -lSDLmain -lSDL -lSDL_ttf -lSDL_image  -lSDL_sound -lSDL_phelma -lSDL_draw -lX11

test1: test1.o son.o 
	gcc -o $@ $^ $(LDFLAGS)


test2: test2.o son.o 
	gcc -o $@ $^ $(LDFLAGS)

test3: test3.o son.o 
	gcc -o $@ $^ $(LDFLAGS)

test4: test4.o son.o 
	gcc -o $@ $^ $(LDFLAGS)


%.o: %.c
	gcc -c $< $(CFLAGS)


all: test1 test2 test3 test4
	
clean :
	rm -f *.o test? *~

	
