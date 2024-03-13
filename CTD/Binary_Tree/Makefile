CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm
RM=rm -f
OBJ=btree.o
EXE=btree
# Pour produire le fichier exécutable
$(EXE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LDLIBS)
# Pour produire les fichiers objets
btree.o:btree.c btree.h
	$(CC) $(CFLAGS) -c btree.c
# Pour effacer les objets et les exécutables
clean:
	$(RM) $(OBJ) $(EXE)
