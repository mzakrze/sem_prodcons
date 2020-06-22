#include "queue.h"
#include <assert.h>
#include <stdio.h>
void init(struct Queue *queue){
	int i;
	queue->size = 0;
	for(i=0;i<100;++i){
		queue->element[i].is_valid = false;
	}
	queue->tail = -1;
}

void show(struct Queue *queue,int i){
	printf("i=%d,prior=%d,next=%d,tail=%d\n",i,queue->element[i].prior,queue->element[i].next,queue->tail);
}

void showall(struct Queue *queue){
	int i=0;
	printf("tail=%d\n",queue->tail);
	for(i=queue->tail;queue->element[i].is_valid;i = queue->element[i].next){
		printf("i=%d,prior=%d,next=%d\n",i,queue->element[i].prior,queue->element[i].next);
	}
}

void push(struct Queue *queue,short prior){
	//printf("wew push,queueu=%d,size=%d\n",queue,queue->size);
	assert(queue->size < 100); // jesli proba dodania do pelnej kolejki
	++queue->size;

	int i=0;
	for(i=0;i<100;++i)
		if(!queue->element[i].is_valid) // szukaj pustego
			break;
	if(queue->tail == -1){ // dodawany pierwszy element
		queue->tail = i;
		queue->element[i].next = -1;
		queue->element[i].prior = prior;
		queue->element[i].is_valid = true;
		return;
	}
	// i wskazuje na pierwszy wolny indeks
	if(prior >= 1){ // dodanie prorytetowego
		// znajdz ostatni priorytetowy
		if(queue->element[queue->tail].prior < 1){
			queue->element[i].next = queue->tail;
			queue->element[i].prior = prior;
			queue->element[i].is_valid = true;
			queue->tail = i;
			return;
		}
		int j = queue->tail;
		int k = queue->element[j].next;
		while(k != -1 && queue->element[k].prior >= 1){ // szukaj ostatniego priorytetowego
			j = queue->element[j].next;
			k = queue->element[j].next;
		}
		// wstaw i-ty element miedzy k-ty i j-ty
		queue->element[i].is_valid = true;
		queue->element[i].next = k;
		queue->element[i].prior = prior;
		queue->element[j].next = i;
	}
	else{ // dodanie niepriorytetowego
		int j = queue->tail;
		while(queue->element[j].next != -1) // znajdz ostatni element
			j = queue->element[j].next;

		queue->element[i].is_valid = true;
		queue->element[i].prior = prior;
		queue->element[i].next = -1;
		queue->element[j].next = i;
	}
	//printf("wychodze z push\n");
}

short pop(struct Queue *queue){
	//printf("wew pop,queue=%d,size=%d\n",queue,queue->size);
	assert(0 != queue->size); // proba czytania z pustej kolejki
	--queue->size;
	short to_return = queue->element[queue->tail].prior;
	queue->element[queue->tail].is_valid = false;
	queue->tail = queue->element[queue->tail].next;
	return to_return;
}