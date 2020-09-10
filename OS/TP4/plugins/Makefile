CC=`which gcc`
LD=$(CC)
RM=`which rm` -f

API=../appli

KERNEL := $(shell uname -s)
ifeq ($(KERNEL), Linux)
	CFLAGS=-Wall -Wextra -g -I$(API)/include -fpic 
	LFLAGS=-shared -pthread -lm
endif
ifeq ($(KERNEL), Darwin)
	CFLAGS=-Wall -Wextra -g -I$(API)/include -fpic 
	LFLAGS=-dynamiclib -undefined dynamic_lookup -lm
endif


PLUGINS_SRC=$(wildcard *.c)
PLUGINS=$(PLUGINS_SRC:.c=.so)

all : $(PLUGINS)

%.so : %.o
	$(LD) $< -o $@ $(LFLAGS) 

%.o : %.c
	$(CC) $(CFLAGS) $< -c

clean :
	$(RM) $(PLUGINS) *~ 
