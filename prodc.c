#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "params.h"
#include "queue.h"
/* producent 2
* produkuje ciasto do pierogow
* produkuje 7 jednostek na sekunde <=> jednostke co 0.1428 sekundy
* umieszcza dane do kolejki 2 - pamieci wspoldzielonej o id shmem2
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
	int shmem2_id;
	int sem0_id,sem2_id;
	struct Queue *queue;
	if(DEBUGG == 0){
		printf("DEBUGG = 0\n");
	}
	if((sem2_id = semget(sem2,3,0600)) < 0){
		perror("blad pobierania id semafora 1\n");
		exit(1);
	}
	if((shmem2_id = shmget(shmem2,sizeof(struct Queue),0666)) < 0){
		perror("blad pobierania id pamieci wspoldzielonej 1\n");
		exit(1);
	}
	if((queue = shmat(shmem2_id,0,0)) <= 0){
		perror("blad przydzielania pamieci wspoldzielonej 1\n");
		exit(1);
	}
	int i=0;
	while(1){		
		down(sem2_id,prod_sem);
		lock_queue(sem2_id);
		up(sem2_id,kons_sem);
		push(queue,0);
		unlock_queue(sem2_id);
		if(DEBUGG == 0)
			printf("c%d\n",i++);
		usleep(143000); // co 1/7 sekundy
	}
	return 0;
}