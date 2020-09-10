CFLAGS=-c -g  -Wall
OBJECTS = testtri.o tas.o
LDFLAGS = -lm

testtri: $(OBJECTS)
	gcc  -o testtri $(OBJECTS) $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $< 
clean:
	rm *.o 

