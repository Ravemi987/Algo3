#ifndef H_LISTE_CHAINEE
#define H_LISTE_CHAINEE


typedef struct _List List;
typedef struct _Maillon Maillon;

 struct _Maillon {
	int value;
	struct _Maillon *previous;
	struct _Maillon *next;
};

struct _List {

	Maillon *sentinel;
	int size;

};

List *list();

List *push_back(List *l, int v);

List *push_front(List *l, int v);

List *pop_front(List *l);

List *pop_front(List *l);

bool list_empty (const List *l);

int front(const List *l);

int back(const List *l);

int at(const List *l, int p);

List *insert_at(List *l , int p, int v);

List *remove_at(List *l , int p);


#endif
