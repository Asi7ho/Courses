BIN=dechiffrage

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

CC=`which gcc`
LD=`which gcc`
RM=`which rm` -f

CFLAGS=-Wall -O2
LFLAGS=

all : $(BIN)

$(BIN) : $(OBJ)
	$(LD) $(LFLAGS) $^ -o $@ 

.c.o : 
	$(CC) $(CFLAGS) $< -c

clean : 
	$(RM) $(OBJ) $(BIN) *~

exe : 
	./$(BIN)
