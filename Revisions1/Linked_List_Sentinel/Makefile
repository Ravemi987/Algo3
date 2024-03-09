CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm
RM=rm -f
OBJ=linked_list.o
EXE=linked_list
# Pour produire le fichier exécutable
$(EXE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LDLIBS)
# Pour produire les fichiers objets
linked_list.o:linked_list.c linked_list.h
	$(CC) $(CFLAGS) -c linked_list.c
# Pour effacer les objets et les exécutables
clean:
	$(RM) $(OBJ) $(EXE)
