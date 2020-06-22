#define DEBUGG 0 // 0 - wypisuj podczas tworzenia, 1 - wypisuj co 0.1 sek stan kolejek

#define kons_sem 1
#define prod_sem 2
const int sem0 = 1000; // zamek do pisania na standardowe wyjscie
const int sem1 = 1001; 
const int sem2 = 1002;
const int sem3 = 1003;

const int shmem1 = 1004;
const int shmem2 = 1005;
const int shmem3 = 1006;

/* 
* zestaw semaforow sklada sie z 3 semaforow:
* 0) zabezpiecza przez jednoczesnym odczytem i zapisem przez konsumenta/producenta
* 1) zabezpiecza przed czytaniem z pustej kolejki - ograniczenie dla konsumenta - na poczatku wartosc 0
* 2) zabezpiecza przed dorzucaniem do pelnej kolejki - ograniczenie dla producenta - na poczatku wartosc 100
*/