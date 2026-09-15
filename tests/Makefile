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

# We want that each file (whatever its path) becomes build/*.o
# 'notdir' keeps just the name of the file, 'patsubst' adds 'build'.
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRC)))

# ------------------------------------------------------------------
# Rules
# ------------------------------------------------------------------

all: $(EXEC)

debug: CFLAGS += -DDDebug -g
debug: LDFLAGS += -DDDebug -g
debug: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -o $@ $^ $(LDFLAGS)

# VPATH : where make has to kook for the .c (this folder and sub-folders of 
# ../src)
VPATH := $(sort $(dir $(SRC)))

# generic rule: build/file.o from file.c found via VPATH
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAGS)

.PHONY: cleanSimple clean

cleanSimple:
	rm -rf $(BUILD_DIR)

clean: cleanSimple
	rm -rf $(EXEC)
