CFLAGS=-c -g 
OBJECTS = liste.o divers.o verif.o

verif: $(OBJECTS)
	gcc  -o verif $(OBJECTS)

%.o: %.c
	gcc $(CFLAGS) $< 
clean:
	rm *.o 
