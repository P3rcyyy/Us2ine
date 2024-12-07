#ifndef MAIN_H
#define MAIN_H

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

// Définition des ressources
typedef struct Ressources {
    int bois;
    int pierre;
    int fer;
    int or;
    int diamant;
    sem_t semaphore;
} Ressources;

// Constantes pour identifier les types de ressources
enum {
    BOIS,
    PIERRE,
    FER,
    OR,
    DIAMANT
};

// Prototypes des fonctions
void *thread_ouvrier(void *arg);
void clearScreen();
void recupererRessources();
void vendreRessources();
void gererOuvriers();
void fabriquerOutils();
void ameliorerOuvriers();
void affecterOuvriers();
void vendreQuantite(int ressource);
void fabriquerPioche();
void fabriquerHache();
void afficherRessources(Ressources *ressources);

#endif // MAIN_H
