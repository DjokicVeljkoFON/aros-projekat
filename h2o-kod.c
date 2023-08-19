#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define MOLEKULI 10

int atomiKiseonikaSpremniZaVezu = 0;
int atomiVodonikaSpremniZaVezu = 0;
int atomiSpremniZaVezu = 0;

sem_t mutex;
sem_t semKiseonik;
sem_t semVodonik;


int funkcijaVeze() {

    atomiSpremniZaVezu++;

    if(atomiSpremniZaVezu % 3 == 0) {
        printf("\nKreiran je novi molekul H2O\n");
        atomiSpremniZaVezu = 0;
    }

    sleep(1);
    return 0;

}

void* funkcijaKiseonika() {

        sleep(1);

        sem_wait(&mutex);
        atomiKiseonikaSpremniZaVezu+=1;

        if(atomiVodonikaSpremniZaVezu>=2) {

            sem_post(&semKiseonik);
            atomiKiseonikaSpremniZaVezu-=1;

            sem_post(&semVodonik);
            sem_post(&semVodonik);
            atomiVodonikaSpremniZaVezu-=2;

        }
        else {

            sem_post(&mutex);

        }
        
        sem_wait(&semKiseonik);
        printf("Molekul O je spreman za H20\n");

        funkcijaVeze();
        sem_post(&mutex);

}

void* funkcijaVodonika() {

        sleep(1);

        sem_wait(&mutex);
        atomiVodonikaSpremniZaVezu+=1;

        if(atomiVodonikaSpremniZaVezu>=2 && atomiKiseonikaSpremniZaVezu>=1) {

            sem_post(&semVodonik);
            sem_post(&semVodonik);
            atomiVodonikaSpremniZaVezu-=2;

            sem_post(&semKiseonik);
            atomiKiseonikaSpremniZaVezu-=1;

        }
        else {

            sem_post(&mutex);

        }

        sem_wait(&semVodonik);
        printf("Molekul H je spreman za H20\n");
        funkcijaVeze();

}

main() {

    printf("Program počinje sa radom\n");
    
    sem_init(&mutex,0,1);
    sem_init(&semKiseonik,0,0);
    sem_init(&semVodonik,0,0);

    printf("Semafori su inicijalizovni\n");        
    sleep(1);

    pthread_t nitiKiseonika[MOLEKULI];
    pthread_t nitiVodonika[2*MOLEKULI];

    for (int i=0; i<MOLEKULI; i++) {
        pthread_create(&nitiKiseonika[i], NULL, funkcijaKiseonika, NULL);
    }
    
    for (int i=0; i<2*MOLEKULI; i++) {
        pthread_create(&nitiVodonika[i], NULL, funkcijaVodonika, NULL);
    }

    printf("\n\n\nNiti su kreirane\n\n\n");        
    sleep(1);

    for(int i = 0; i < MOLEKULI; i++) {
        pthread_join(nitiKiseonika[i], NULL);
    }

    for(int i = 0; i < 2*MOLEKULI; i++) {
        pthread_join(nitiVodonika[i], NULL);
    }

    printf("\n\nProgram je završio sa radom");

}