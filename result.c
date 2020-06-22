#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "params.h"
#include "queue.h"

int main(){
	struct Queue *q1,*q2,*q3;
	int shmem1_id,shmem2_id,shmem3_id;
	if(DEBUGG == 0)
		return;
	if((shmem1_id = shmget(shmem1,sizeof(struct Queue),0666)) < 0){
		perror("blad pobierania id pamieci wspoldzielonej 1\n");
		exit(1);
	}
	if((shmem2_id = shmget(shmem2,sizeof(struct Queue),0666)) < 0){
		perror("blad pobierania id pamieci wspoldzielonej 2\n");
		exit(1);
	}
	if((shmem3_id = shmget(shmem3,sizeof(struct Queue),0666)) < 0){
		perror("blad pobierania id pamieci wspoldzielonej 3\n");
		exit(1);
	}
	if((q1 = shmat(shmem1_id,0,0)) <= 0){
		perror("blad przydzielania pamieci wspoldzielonej 1\n");
		exit(1);
	}
	if((q2 = shmat(shmem2_id,0,0)) <= 0){
		perror("blad przydzielania pamieci wspoldzielonej 2\n");
		exit(1);
	}
	if((q3 = shmat(shmem3_id,0,0)) <= 0){
		perror("blad przydzielania pamieci wspoldzielonej 3\n");
		exit(1);
	}
	while(1){
		usleep(100000); // update co 0.1 sekundy
		printf("q1:%d,q2:%d,q3:%d\n",q1->size,q2->size,q3->size);
	}
}
