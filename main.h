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

// Structure du message
struct message {
    long message_type;
    int signal;
};

// Clé pour la boîte aux lettres
#define MESSAGE_QUEUE_KEY 7171

// Définition des ressources
typedef struct Ressources {
    int bois;
    int pierre;
    int fer;
    int or;
    int diamant;
    int argent;
    sem_t semaphore;
} Ressources;

// Structure représentant les coûts de fabrication
typedef struct {
    int bois;    // Quantité de bois nécessaire
    int pierre;  // Quantité de pierre nécessaire
    int fer;     // Quantité de fer nécessaire
    int or;      // Quantité d'or nécessaire
    int diamant; // Quantité de diamant nécessaire
    int argent;  // Quantité d'argent nécessaire
} CoutFabrication;


// Constantes pour identifier les types de ressources
typedef enum {
    BOIS,
    PIERRE,
    FER,
    OR,
    DIAMANT,
    ARGENT, // Ressource utilisée pour l'argent du jeu
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
    int niveauHache;     // Niveau de la hache (0 = pas de hache)
    int niveauPioche;    // Niveau de la pioche (0 = pas de pioche)
    int recolteTemps;    // Temps pour récolter une ressource
} Joueur;



// Prototypes des fonctions
void *thread_ouvrier(void *arg);
void clearScreen();
void recupererRessources(Joueur *joueur, Ressources *ressources);
void vendreRessources();
void gererOuvriers();
void ameliorerOuvriers();
void affecterOuvriers();
void vendreQuantite(int ressource);
void fabriquerOutils(Joueur *joueur, Ressources *ressources);
void fabriquerPioche(Joueur *joueur, Ressources *ressources);
void fabriquerHache(Joueur *joueur, Ressources *ressources);
void afficherRessources(Ressources *ressources);
const char* getNomRessource(RessourceType type);
void recolterRessource(Joueur *joueur, Ressources *ressources, RessourceType type);


#endif // MAIN_H
