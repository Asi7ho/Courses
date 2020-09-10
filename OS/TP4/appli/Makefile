CC=`which gcc`
LD=$(CC)
RM=`which rm` -f

KERNEL := $(shell uname -s)
ifeq ($(KERNEL), Linux)
	CFLAGS=-Wall -Wextra -g -I./include/ 
	LFLAGS=-Wl,--export-dynamic -ldl # DSO magic for Linux
endif
ifeq ($(KERNEL), Darwin)
	CFLAGS=-Wall -Wextra -g -I./include/ 
	LFLAGS=
endif

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)


TARGET=shell

all : $(TARGET)


$(TARGET) : $(OBJ)
	$(LD) $^ -o $@ $(LFLAGS) 

%.o : %.c
	$(CC) $(CFLAGS) $< -c

clean : 
	$(RM) $(BIN) $(OBJ) *~ semantic.cache

exe : 
	./$(BIN)
