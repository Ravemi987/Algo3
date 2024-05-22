CC=gcc
CFLAGS=-std=c99 -Wextra -Wall -pedantic 
LDFLAGS=-lm

ifeq ($(DEBUG),yes)
	CFLAGS += -g
	LDFLAGS +=
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS +=
endif

EXEC=tree_test
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: 
ifeq ($(DEBUG),yes)
	@echo "Generating in debug mode"
else
	@echo "Generating in release mode"
endif
	@$(MAKE) $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC) ../Test/*.dot ../Test/*.pdf

pdf : $(EXEC)
	@dot -Tpdf ../Test/*.dot -O
	
tree.o : tree.h
main.o : tree.o
doc : tree.h  main.c
