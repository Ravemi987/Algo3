CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm
RM=rm -f
OBJ=TestStack.o Stack.o
EXE=TestStack
# Pour produire le fichier exécutable
$(EXE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LDLIBS)
# Pour produire les fichiers objets
TestStack.o:TestStack.c Stack.h
	$(CC) $(CFLAGS) -c TestStack.c
StaticStack.o:StaticStack.c Stack.h
	$(CC) $(CFLAGS) -c Stack.c
# Pour effacer les objets et les exécutables
clean:
	$(RM) $(OBJ) $(EXE)
