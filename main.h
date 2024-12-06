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

// Définition de l'énumération des ressources
typedef enum {
    BOIS,
    PIERRE,
    FER,
    OR,
    DIAMANT,
    NB_RESSOURCES // Nombre total de ressources
} Ressource;

// Déclaration des fonctions
void clearScreen();
void recupererRessources();
void vendreRessources();
void gererOuvriers();
void fabriquerOutils();
void ameliorerOuvriers();
void affecterOuvriers();
void vendreQuantite(Ressource ressource);
void fabriquerPioche();
void fabriquerHache();

#endif // MAIN_H
