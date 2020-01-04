#include <array>
#include <atomic>
#include <semaphore.h>
#include <zconf.h>


const int liczbaFilozofow = 5;

sem_t paleczki[liczbaFilozofow];


int prawaPaleczka(int i) {
    return (i + 1) % 5;
}

int lewaPaleczka(int i) {
    return i;
}


void *filozofJe(void *arg) {

    bool je = true;
    int id = (int) (uintptr_t) arg;
    while (je) {
        usleep(10000000);

        if (lewaPaleczka(id) < prawaPaleczka(id)) {
            printf("Filozof %i bierze lewą pałeczkę\n", id);
            sem_wait(&paleczki[lewaPaleczka(id)]);
            printf("Filozof %i bierze prawą pałeczkę\n", id);
            sem_wait(&paleczki[prawaPaleczka(id)]);
        }
        else {
            printf("  Filozof %i bierze prawą pałeczkę\n", id);
            sem_wait(&paleczki[prawaPaleczka(id)]);
            printf("  Filozof %i bierze lewą pałeczkę\n", id);
            sem_wait(&paleczki[lewaPaleczka(id)]);
        }

        printf("Filozof %i filozof wzial obie pałeczki\n", id);

        // Filozof je
        printf("Filozof %i, je!\n", id);
        je = false;
        usleep(1000000);

        // Rozwiazanie z hirarchia paleczek

        if (lewaPaleczka(id) < prawaPaleczka(id)) {
            printf("Filozof %i kladzie prawą pałeczkę\n", id);
            sem_post(&paleczki[prawaPaleczka(id)]);
            printf("Filozof %i kladzie lewą pałeczkę\n", id);
            sem_post(&paleczki[lewaPaleczka(id)]);
        }
        else {
            printf("Filozof %i kladzie lewą pałeczkę\n", id);
            sem_post(&paleczki[lewaPaleczka(id)]);
            printf("Filozof %i kladzie prawą pałeczkę\n", id);
            sem_post(&paleczki[prawaPaleczka(id)]);
        }
        printf("***Filozof %i położył obie pałeczki i skończył jedzenie.***\n", id);
    }
    return nullptr;
}

int main() {

    auto *watekFilozofow = static_cast<pthread_t *>(malloc(liczbaFilozofow * sizeof(pthread_t))); // rzutowanie

    for (int i = 0; i < liczbaFilozofow; ++i) {
        sem_init(&paleczki[i], 0, 1);
        pthread_create(&watekFilozofow[i], nullptr, filozofJe, (void *) (uintptr_t) i);
    }


    for (int i = 0; i < liczbaFilozofow; ++i) {
        pthread_join(watekFilozofow[i], nullptr);
    }

    free(watekFilozofow);
    for (int i = 0; i < liczbaFilozofow; ++i) {
        sem_destroy(&paleczki[i]);
    }

    return 0;
}