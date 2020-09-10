CFLAGS=-c -g 
LDFLAGS=

test_liste0: test_liste0.o liste.o carte.o
	gcc  -o $@ $^ $(LDFLAGS)

test_liste1: test_liste1.o liste.o carte.o
	gcc  -o $@ $^ $(LDFLAGS)

test_liste2: test_liste2.o liste.o carte.o
	gcc  -o $@ $^ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) $< 

