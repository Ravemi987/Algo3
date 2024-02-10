#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_SIZE 16

typedef struct {

    int queue[MAX_SIZE];
	int head;
	int tail;
	unsigned int size;

} Queue;


Queue *queue(){
	Queue *q = malloc(sizeof(Queue));
	q->head = 0;
    q->tail = -1;
	q->size = 0;

	return(q);
}

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
