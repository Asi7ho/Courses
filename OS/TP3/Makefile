CC=`which gcc`
LD=$(CC)
RM=`which rm` -f


SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

CFLAGS=-Wall -Wextra -g -pedantic-errors -I./include/ -DUSE_$(ALLOC) -DTEST_$(TEST) 
LFLAGS=

TARGET=palloc-test

all : $(TARGET)


$(TARGET) : $(OBJ)
	@$(LD) $(LFLAGS) $^ -o $@ 

%.o : %.c
	@$(CC) $(CFLAGS) $< -c

clean : 
	@$(RM) $(BIN) $(OBJ) *~ semantic.cache

exe : 
	./$(BIN)
