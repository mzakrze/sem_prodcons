#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include "params.h"
#include "queue.h"
#include <assert.h>

int main()
{
    struct Queue *q1,*q2,*q3;
    int shm1_id,shm2_id,shm3_id;
    int sem0_id,sem1_id,sem2_id,sem3_id;
    /* alokuj pamiec na kolejke 1 */
    if ((shm1_id = shmget(shmem1,sizeof(struct Queue), IPC_CREAT | 0666)) < 0) {
        perror("shmget\n");
        exit(1);
    }
    if ((q1 = shmat(shm1_id, NULL, 0)) == -1) {
        perror("shmat\n");
        exit(1);
    }
    /* alokuj pamiec na kolejke 2 */
    if ((shm2_id = shmget(shmem2,sizeof(struct Queue), IPC_CREAT | 0666)) < 0) {
        perror("shmget\n");
        exit(1);
    }
    if ((q2 = shmat(shm2_id, NULL, 0)) == -1) {
        perror("shmat\n");
        exit(1);
    }
    /* alokuj pamiec na kolejke 3 */
    if ((shm3_id = shmget(shmem3,sizeof(struct Queue), IPC_CREAT | 0666)) < 0) {
        perror("shmget\n");
        exit(1);
    }
    if ((q3 = shmat(shm3_id, NULL, 0)) == -1) {
        perror("shmat\n");
        exit(1);
    }
    /* inicjuj kolejki */
    init(q1);
    init(q2);
    init(q3);
    /* stworz i inicjuj zestaw semaforow 0 */
    sem0_id = semget(sem0,1,IPC_CREAT|0600);
    semctl(sem0_id,0,SETVAL,(int)1); // semafor 0 zabezpiecza przez wyscigami z pisaniem na standardowe wyjscie
    /* stworz i inicjuj zestaw semaforow 1 */
    sem1_id = semget(sem1,3,IPC_CREAT|0600);
    semctl(sem1_id,0,SETVAL,(int)1); 
    semctl(sem1_id,1,SETVAL,(int)0); // na poczatku konsument ma 0 zasobow(danych w kolejce)
    semctl(sem1_id,2,SETVAL,(int)100); // na poczatku producent ma 100 zasobow(miejsca na dane w kolejce)
    /* stworz i inicjuj zestaw semaforow 1 */
    sem2_id = semget(sem2,3,IPC_CREAT|0600);
    semctl(sem2_id,0,SETVAL,(int)1); 
    semctl(sem2_id,1,SETVAL,(int)0); // na poczatku konsument ma 0 zasobow(danych w kolejce)
    semctl(sem2_id,2,SETVAL,(int)100); // na poczatku producent ma 100 zasobow(miejsca na dane w kolejce)
    /* stworz i inicjuj zestaw semaforow 1 */
    sem3_id = semget(sem3,3,IPC_CREAT|0600);
    semctl(sem3_id,0,SETVAL,(int)1); 
    semctl(sem3_id,1,SETVAL,(int)0); // na poczatku konsument ma 0 zasobow(danych w kolejce)
    semctl(sem3_id,2,SETVAL,(int)100); // na poczatku producent ma 100 zasobow(miejsca na dane w kolejce)

    return 0;
}