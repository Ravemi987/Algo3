#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "liste_chainee.h"



//Creation d’une liste doublement chaˆın ́ee vide
List  *list(){
	List *l = malloc(sizeof(List));
	l->head = l->tail = NULL;
	l->size = 0;
	return l;
}


//Operateurs d’ajout et de suppression d’un élément à une liste doublement chaînée
List *push_back(List *l, int v){
	Maillon **insert_at = (l->size ? &(l->tail->next) : &(l->head));
	Maillon *new = malloc(sizeof(Maillon));
	new->value = v;
	new->next = 

}


//Opérateurs d'état du TAD Liste
bool list_empty (const List *l){
	return l->size == 0;
}


List *pop_front(List *l){
//TODO
}


int front(const List *l){
	assert(!list_empty(l));
	return l->head->value ;
}

int back(const List *l){
	assert(!list_empty(l));
	return l->tail->value ;
}


int at(const List *l, int p){
//TODO
	return(0)
}


List *insert_at(List *l , int p, int v){
//TODO

}






int main(){

	List* l = list();

	int i;

	//l'indice démarre à 0 !
	//for(i=0;i<10;i++) l = push_back(l,i);
	//for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );

	printf("###########################\n");
    printf("###########################\n");
   // printf("size  %d \n",l->size);
    printf("###########################\n");

	printf("cas 1 : ajoute en fin de liste\n");
	//l =  insert_at(l, l->size,100);	
	//for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );


	printf("###########################\n");

	printf("cas 2 :ajout liste vide (création nouvelle liste) \n");
	//List* l2 = list();
	//l2 =  insert_at(l2, 0,100);	
	//printf("at %d\n",at(l2,0) );

	printf("###########################\n");


	printf("cas 2.2 :ajout début de liste \n");
//	l =  insert_at(l, 0,200);	
//	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );


	printf("###########################\n");


//	printf("cas 3 :ajout au milieu \n");
//	l =  insert_at(l, 2,300);	
// 	for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );

	printf("###########################\n");



	printf("###########################\n");	
	printf("###########################\n");
    printf("suppression \n");

    printf("###########################\n");
    printf("Suppression 1er élément \n");
//    l =  remove_at(l, 0);
////    for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );
    printf("###########################\n");
////    printf("size  %d \n",l->size);
    printf("###########################\n");
    printf("Suppression dernier élément \n");
////    l =  remove_at(l, l->size);
////    for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );
    printf("###########################\n");
    printf("Suppression élément indice 1 \n");
////    l =  remove_at(l, 1);
 ////   for(i=0;i<l->size;i++) printf("at %d\n",at(l,i) );

return(0);
}



// val = exp1 ? exp 2 : exp 3 
// if exp1 = 0 val = exp2 else val = exp3
// Exemple 
// (a%2==0)? 1:0; renvoi 1 si a est divisible par 2 sion 0
// (a>b) ? ((a>c)?a:c):((b>c)?b:c) renvoi la plus grande valeur entre a,b et c
// (x>y) ? x--:y--
