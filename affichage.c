#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include "main.h"

int main()
{
    const char *pathname = "README.md"; // Même fichier que celui utilisé dans le main
    key_t key = ftok(pathname, 7171);
    if (key == -1)
    {
        perror("ftok");
        return 1;
    }

    int shmid = shmget(key, sizeof(Ressources), 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }

    Ressources *ressources = (Ressources *)shmat(shmid, NULL, 0);
    if (ressources == (void *)-1)
    {
        perror("shmat");
        return 1;
    }

    while (1)
    {
        sem_wait(&ressources->semaphore);
        system("clear"); // Efface l'écran pour un affichage propre
        printf("=== Affichage des Ressources ===\n");
        printf("Bois     : %d\n", ressources->bois);
        printf("Pierre   : %d\n", ressources->pierre);
        printf("Fer      : %d\n", ressources->fer);
        printf("Or       : %d\n", ressources->or);
        printf("Diamant  : %d\n", ressources->diamant);
        sem_post(&ressources->semaphore);
        sleep(1); // Attendre 1 secondes
    }

    return 0;
}
