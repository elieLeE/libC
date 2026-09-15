#Makefile
#cible: dependances
#	commandes
#$@ : cible
#$< : nom de la premiere dependance
#$^ : liste des dependances
#$? : liste des dependances plus recentes que la cible
#$* : nom du fichier sans le suffixe

CC := gcc
CFLAGS := -std=gnu2x -Wextra -Wall -g
LDFLAGS := -lm -lgmp
INCLUDE := -I. -I../src

BUILD_DIR := ./build
EXEC := test

# ------------------------------------------------------------------
# Sources :
# - test files in this folder (test_*.c)
# - source files in ../src/...
# ------------------------------------------------------------------
LIB_C := ../src

SRC := $(wildcard *.c \
	$(LIB_C)/containers/*.c \
	$(LIB_C)/mem/mem.c \
	$(LIB_C)/logger/logger.c \
	$(LIB_C)/io/io.c \
	$(LIB_C)/test_helper/test_helper.c \
	$(LIB_C)/math/*.c \
	$(LIB_C)/tab_helper/tab_helper.c \
	$(LIB_C)/utils.c)
OBJ := $(SRC:.c=.o)

all: $(EXEC)

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
