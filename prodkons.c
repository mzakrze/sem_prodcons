#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "params.h"
#include "queue.h"
/* producent & konsument
* pobiera ciasto z kolejki 2 oraz farsz z kolejki 1
* wykonuje 4 operacje na sekunde <=> operacja co 0.25 sekundy
* umieszcza dane do kolejki 3 - pamieci wspoldzielonej o id shmem3
*/

static struct sembuf buf;

void lock_queue(int semid){
	buf.sem_op = -1;
	buf.sem_num = 0;
	buf.sem_flg = 0;
	if(-1 == semop(semid,&buf,1)){
		printf("blad lockowania\n");
		exit(1);
	}
}

void unlock_queue(int semid){
	buf.sem_op = 1;
	buf.sem_num = 0;
	buf.sem_flg = 0;
	if(-1 == semop(semid,&buf,1)){
		printf("blad unlockowania\n");
		exit(1);
	}
}

void down(int semid,int semnum){
	buf.sem_num = semnum;
	buf.sem_op = -1; // kod operacji opuszczania semafora
	buf.sem_flg = 0; // bez flag ( ? )
	if(-1 == semop(semid,&buf,1)){ // 3 arg - liczba elementów tablicy operacji semaforowych ( ? )
		printf("blad opuszczania semafora,semid=%d,errno=%d\n",semid,errno);
		printf("%s",strerror(errno));
	}
}

void up(int semid,int semnum){
	buf.sem_num = semnum;
	buf.sem_op = 1; // kod operacji podnoszenia semafora
	buf.sem_flg = 0; // bez flag ( ? )
	if(-1 == semop(semid,&buf,1)){ // 3 arg - liczba elementów tablicy operacji semaforowych ( ? )
		printf("blad opuszczania semafora,semid=%d,errno=%d\n",semid,errno);
		printf("%s",strerror(errno));
	}
}


int main(){
	int shmem1_id,shmem2_id,shmem3_id;
	int sem0_id,sem1_id,sem2_id,sem3_id;
	struct Queue *q1,*q2,*q3;
	if((sem1_id = semget(sem1,3,0600)) < 0){
		perror("blad pobierania id semafora 1\n");
		exit(1);
	}
	if((sem2_id = semget(sem2,3,0600)) < 0){
		perror("blad pobierania id semafora 2\n");
		exit(1);
	}
	if((sem3_id = semget(sem3,3,0600)) < 0){
		perror("blad pobierania id semafora 3\n");
		exit(1);
	}
	/********************/
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
	/*********************/
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

	int i=0;
	int prior;
	while(1){		
		usleep(250000); // co 1/4 sekundy
		down(sem1_id,kons_sem);
		down(sem2_id,kons_sem);
		down(sem3_id,prod_sem);
		lock_queue(sem1_id); lock_queue(sem2_id); lock_queue(sem3_id);
		up(sem1_id,prod_sem);
		up(sem2_id,prod_sem);
		up(sem3_id,kons_sem);
		// zdejmij z kolejki 1 i 2
		prior = pop(q1);
		pop(q2);
		// pushnij na kolejke 3 z priorytetem takim jak ten zdjety z kolejki 1
		push(q3,prior);
		// podnies zamek kolejek 1,2,3
		unlock_queue(sem1_id); unlock_queue(sem2_id); unlock_queue(sem3_id);
		if(DEBUGG == 0)
			if(prior)
				printf("p%dp\n",i++);
			else
				printf("p%dn\n",i++);
	}
	return 0;
}