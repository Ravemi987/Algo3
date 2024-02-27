CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm
RM=rm -f
OBJ=iterator.o
EXE=iterator
# Pour produire le fichier exécutable
$(EXE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LDLIBS)
# Pour produire les fichiers objets
iterator.o:iterator.c iterator.h
	$(CC) $(CFLAGS) -c iterator.c
# Pour effacer les objets et les exécutables
clean:
	$(RM) $(OBJ) $(EXE)
