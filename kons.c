#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "params.h"
#include "queue.h"
/* dyspozytornia
* usuwa pierogi z modelu
* usuwa 3 pierogi na sekunde <=> 1 co 0.333 sekundy 
* pobiera dane z kolejki 3 i usuwa je z modelu
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
	int shmem3_id;
	int sem0_id,sem3_id;
	struct Queue *queue;
	if((sem3_id = semget(sem3,3,0600)) < 0){
		perror("blad pobierania id semafora 1\n");
		exit(1);
	}
	if((shmem3_id = shmget(shmem3,sizeof(struct Queue),0666)) < 0){
		perror("blad pobierania id pamieci wspoldzielonej 1\n");
		exit(1);
	}
	if((queue = shmat(shmem3_id,0,0)) <= 0){
		perror("blad przydzielania pamieci wspoldzielonej 1\n");
		exit(1);
	}
	int i=0;
	int prior;
	while(1){
		usleep(333000); // co 0.333 sek
		down(sem3_id,kons_sem);
		lock_queue(sem3_id);
		up(sem3_id,prod_sem);
		prior = pop(queue);
		unlock_queue(sem3_id);
		if(DEBUGG == 0)
			if(prior)
				printf("d%dp\n",i++);
			else
				printf("d%dn\n",i++);
	}
	return 0;
}