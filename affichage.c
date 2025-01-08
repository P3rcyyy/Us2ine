#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include "main.h"

// Définition des couleurs
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define MARRON "\033[38;5;130m"
#define GRIS_FONCE "\033[90m"
#define GRIS_CLAIR "\033[37m"
#define JAUNE_OR "\033[38;5;220m"
#define CYAN "\033[36m"
#define VERT "\033[32m"

void print_header()
{
    printf("\n%s=========================================%s\n", BOLD, RESET);
    printf("%s|      🛠️  Gestion des Ressources      |\n", CYAN);
    printf("%s=========================================%s\n\n", BOLD, RESET);
}

void print_table(Ressources *ressources)
{
    printf("%s=========================================%s\n", BOLD, RESET);
    printf("%s%-12s %-12s%s\n", BOLD, "Ressource", "Quantité", RESET);
    printf("%s-----------------------------------------%s\n", BOLD, RESET);

    printf("%s%-12s%s %d\n", MARRON, "Bois", RESET, ressources->bois);
    printf("%s%-12s%s %d\n", GRIS_FONCE, "Pierre", RESET, ressources->pierre);
    printf("%s%-12s%s %d\n", GRIS_CLAIR, "Fer", RESET, ressources->fer);
    printf("%s%-12s%s %d\n", JAUNE_OR, "Or", RESET, ressources->or);
    printf("%s%-12s%s %d\n", CYAN, "Diamant", RESET, ressources->diamant);
    printf("%s%-12s%s %d\n", VERT, "Argent", RESET, ressources->argent);

    printf("%s=========================================%s\n", BOLD, RESET);
}


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

        print_header();
        print_table(ressources);

        sem_post(&ressources->semaphore);
        sleep(1); // Attendre 1 seconde
    }

    return 0;
}
