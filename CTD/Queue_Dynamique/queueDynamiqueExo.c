#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "queueDynamique.h"



 struct d_queue {
	int value;
	struct d_queue *next;
};



struct s_queue{

	 Maillon *head;
	 Maillon *tail;
	 unsigned int size;

};


Queue *queue(){
	Queue *q = malloc(sizeof(Queue));
	q->head = q->tail = NULL;
	q->size = 0;

	return(q);
}

//double pointeur car on veut modifier la valeur du pointeur (et pas la valeur de ce qu'il pointe)
/*
On va utiliser ce qu'on appelle en anglais un Ternary conditional
une expression a? b : c 
veut dire si a est vraie alors renvoyer b, sinon c 

*/
Queue *queue_push(Queue *q, int v){
//TODO
}

Queue *pop(Queue *q){
//TODO
}

//Opérateurs d'état du TAD Queue
bool queue_empty (const Queue *q){
	return q->size == 0;
}

int main(){
	Queue* q = queue();
	int i;
	for(i=1;i<=3;i++){
			q = queue_push(q,i);
			printf("Size %d\n",q->size );
			printf("Head %d\n",q->head->value );
			printf("Tail %d\n",q->tail->value );
	}

		
return(0);
}
