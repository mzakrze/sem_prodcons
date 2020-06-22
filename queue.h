

#include <stdbool.h>
struct Element{
	int next;
	short prior;
	bool is_valid;
};

struct Queue{
	struct Element element[100];
	int size;
	int tail; // indeks ostatniego elementu 
};

void init(struct Queue *queue);

void push(struct Queue *q,short prior);

short pop(struct Queue *q);

void showall(struct Queue *queue);