#Makefile
#cible: dependances
#	commandes
#$@ : cible
#$< : nom de la premiere dependance
#$^ : liste des dependances
#$? : liste des dependances plus recentes que la cible
#$* : nom du fichier sans le suffixe

CC=gcc
CFLAGS=-std=c99 -Wextra -Wall -g# -ansi -pedantic
LDFLAGS=
EXEC=test
LIB_C=../../src
SRC= $(wildcard *.c $(LIB_C)/io/io.c $(LIB_C)/mem/mem.c ../shared/ftest.c\
	 $(LIB_C)/logger/logger.c)
OBJ= $(SRC:.c=.o)
INCLUDE=-I. -I../../src/io -I../../src/mem -I../shared

all: $(EXEC) cleanSimple

debug: CFLAGS += -DDDebug -g
debug: LDFLAGS += -DDDebug -g
debug: $(EXEC) cleanSimple

$(EXEC): $(OBJ)
	$(CC) $(INCLUDE) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(INCLUDE) -o $@ -c $< $(CFLAGS)

.PHONY: cleanSimple clean

cleanSimple:
	rm -rf $(OBJ)

clean: cleanSimple
	rm -rf $(EXEC)	
