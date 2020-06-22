#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "params.h"
#include "queue.h"
/* producent 1p
* produkuje pierogi grzybowe - priorytetowe
* produkuje statystycznie 1 jednostke na 2 sekundy 
* umieszcza dane z priorytetem 1 do kolejki 1 - pamieci wspoldzielonej o id shmem1
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
	srand(time(NULL));
	int shmem1_id;
	int sem0_id,sem1_id;
	struct Queue *queue;
	if((sem1_id = semget(sem1,3,0600)) < 0){
		perror("blad pobierania id semafora 1\n");
		exit(1);
	}
	if((shmem1_id = shmget(shmem1,sizeof(struct Queue),0666)) < 0){
		perror("blad pobierania id pamieci wspoldzielonej 1\n");
		exit(1);
	}
	if((queue = shmat(shmem1_id,0,0)) <= 0){
		perror("blad przydzielania pamieci wspoldzielonej 1\n");
		exit(1);
	}
	init(queue);
	int i=0;
	while(1){
		usleep(rand()%4000000); // czeka od 0 do 4 sekund
		//produce(sem1_id);
		down(sem1_id,prod_sem);
		lock_queue(sem1_id);
		up(sem1_id,kons_sem);
		push(queue,1);
		unlock_queue(sem1_id);
		if(DEBUGG == 0)
			printf("f%dp\n",i++);
	}
	return 0;
}