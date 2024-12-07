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
typedef enum {
    BOIS,
    PIERRE,
    FER,
    OR,
    DIAMANT,
    NB_RESSOURCES // Utilisé pour dimensionner les tableaux
} RessourceType;

// Constantes pour identifier les types d'outils
typedef enum {
    MAIN,
    HACHE,
    PIOCHE
} OutilType;

// Structure représentant un outil
typedef struct {
    OutilType type;  // Type d'outil
    int niveau;      // Niveau de l'outil (1 = bois, 2 = pierre, etc.)
} Outil;

// Structure représentant un joueur
typedef struct {
    Outil outil;     // L'outil actuellement utilisé par le joueur
    int recolteTemps; // Temps pour récolter une ressource
} Joueur;


// Prototypes des fonctions
void *thread_ouvrier(void *arg);
void clearScreen();
void recupererRessources(Joueur *joueur, int *ressources);
void vendreRessources();
void gererOuvriers();
void fabriquerOutils();
void ameliorerOuvriers();
void affecterOuvriers();
void vendreQuantite(int ressource);
void fabriquerPioche();
void fabriquerHache();
void afficherRessources(Ressources *ressources);
const char* getNomRessource(RessourceType type);
void recolterRessource(Joueur *joueur, int *ressources, RessourceType type);


#endif // MAIN_H
