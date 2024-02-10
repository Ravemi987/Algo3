#ifndef __QUEUE__H_
#define __QUEUE__H_

#endif

#include <stdbool.h>

typedef struct s_queue Queue;
typedef struct d_queue Maillon;

Queue* queue();
Queue *queue_push(Queue *q, int v);
Queue *pop(Queue *q);
bool queue_empty (const Queue *q);

