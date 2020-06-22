#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	struct Queue q;
	init(&q);
	push(&q,0);
	push(&q,2);
	push(&q,0);
	push(&q,3);
	push(&q,0);
	push(&q,4);
	push(&q,0);
	push(&q,5);
	showall(&q);
	printf("%d\n",pop(&q));
	printf("%d\n",pop(&q));	
	printf("%d\n",pop(&q));
	printf("%d\n",pop(&q));	
	printf("%d\n",pop(&q));
	printf("%d\n",pop(&q));	
	printf("%d\n",pop(&q));
	printf("%d\n",pop(&q));
	return 0;
}