#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "params.h"
#include "queue.h"
#include <assert.h>


int main(){
	int shm1_id,shm2_id,shm3_id;
	int sem0_id,sem1_id,sem2_id,sem3_id;
	/* zwolnic pamiec na kolejke 1 */
	if((shm1_id = shmget(shmem1,sizeof(struct Queue),0666)) == -1){
		perror("blad pobierania id pamieci 1\n");
		exit(1);
	}
	if(-1 == shmctl(shm1_id,IPC_RMID,NULL)){
		perror("blad usuwania pamieci 1\n");
		exit(1);
	}
	/* zwolnic pamiec na kolejke 2 */
	if((shm2_id = shmget(shmem2,sizeof(struct Queue),0666)) == -1){
		perror("blad pobierania id pamieci 2\n");
		exit(1);
	}
	if(-1 == shmctl(shm2_id,IPC_RMID,NULL)){
		perror("blad usuwania pamieci 2\n");
		exit(1);
	}
	/* zwolnic pamiec na kolejke 3 */
	if((shm3_id = shmget(shmem3,sizeof(struct Queue),0666)) == -1){
		perror("blad pobierania id pamieci 3\n");
		exit(1);
	}
	if(-1 == shmctl(shm3_id,IPC_RMID,NULL)){
		perror("blad usuwania pamieci 3\n");
		exit(1);
	}
	/* zwolnij zestaw semaforow 0 */
	if((sem0_id = semget(sem0,1,0600)) == -1){
		perror("blad pobierania id semafora 0\n");
		exit(1);
	}
	if(-1 == semctl(sem0_id,0,IPC_RMID)){ // 2arg jest ignorowany
		perror("blad zwalniania semafora 0\n");
		exit(1);
	}
	/* zwolnij zestaw semaforow 1 */
	if((sem1_id = semget(sem1,1,0600)) == -1){
		perror("blad pobierania id semafora 0\n");
		exit(1);
	}
	if(-1 == semctl(sem1_id,0,IPC_RMID)){ // 2arg jest ignorowany
		perror("blad zwalniania semafora 0\n");
		exit(1);
	}
	/* zwolnij zestaw semaforow 2 */
	if((sem2_id = semget(sem2,1,0600)) == -1){
		perror("blad pobierania id semafora 0\n");
		exit(1);
	}
	if(-1 == semctl(sem2_id,0,IPC_RMID)){ // 2arg jest ignorowany
		perror("blad zwalniania semafora 0\n");
		exit(1);
	}
		/* zwolnij zestaw semaforow 3 */
	if((sem3_id = semget(sem3,1,0600)) == -1){
		perror("blad pobierania id semafora 0\n");
		exit(1);
	}
	if(-1 == semctl(sem3_id,0,IPC_RMID)){ // 2arg jest ignorowany
		perror("blad zwalniania semafora 0\n");
		exit(1);
	}
}