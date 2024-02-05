CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm
RM=rm -f
OBJ=TestStaticStack.o StaticStack.o
EXE=TestStaticStack
# Pour produire le fichier exécutable
$(EXE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LDLIBS)
# Pour produire les fichiers objets
TestStaticStack.o:TestStaticStack.c StaticStack.h
	$(CC) $(CFLAGS) -c TestStaticStack.c
StaticStack.o:StaticStack.c StaticStack.h
	$(CC) $(CFLAGS) -c StaticStack.c
# Pour effacer les objets et les exécutables
clean:
	$(RM) $(OBJ) $(EXE)
