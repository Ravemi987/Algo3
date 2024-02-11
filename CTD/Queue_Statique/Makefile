CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra
LDLIBS=-lm
RM=rm -f
OBJ=TestQueue.o Queue.o
EXE=TestQueue
# Pour produire le fichier exécutable
$(EXE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LDLIBS)
# Pour produire les fichiers objets
TestQueue.o:TestQueue.c Queue.h
	$(CC) $(CFLAGS) -c TestQueue.c
Queue.o:Queue.c Queue.h
	$(CC) $(CFLAGS) -c Queue.c
# Pour effacer les objets et les exécutables
clean:
	$(RM) $(OBJ) $(EXE)
