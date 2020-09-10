CFLAGS=-c -g 
LDFLAGS= -lm
OBJECTS=    main.o fichiers.o graphe.o liste.o fourmis.o file.o 

main: $(OBJECTS)
	gcc  -o main $(OBJECTS) $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $<

clean: 
	rm *.o

