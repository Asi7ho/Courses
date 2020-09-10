CFLAGS=-c -g 
LDFLAGS=

test_pile: test_pile.o pile.o liste.o carte.o
	gcc  -o $@ $^ $(LDFLAGS)

test_file: test_file.o file.o liste.o carte.o
	gcc  -o $@ $^ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $< 

