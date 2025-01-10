#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
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
#define ROUGE "\033[31m"

// Symboles pour les ressources
#define BOIS_ICON "🌲"
#define PIERRE_ICON "🪨"
#define FER_ICON "⛓️"
#define ARGENT_ICON "💰"
#define DIAMANT_ICON "💎"
#define OR_ICON "🪙"

// Clé pour la boîte aux lettres
#define MESSAGE_QUEUE_KEY 7171

// Fonction d'affichage du titre
void print_header()
{
    system("clear");
    printf("\n%s=========================================%s\n", BOLD, RESET);
    printf("|      🛠️  GESTION DES RESSOURCES      |\n");
    printf("%s=========================================%s\n\n", BOLD, RESET);
}

// Fonction pour afficher la table des ressources
void print_table(Ressources *ressources)
{
    printf("%s┌───────────────────────┬──────────────────────┐%s\n", GRIS_CLAIR, RESET);
    printf("%s│   Ressource           │   Quantité           │%s\n", BOLD, RESET);
    printf("%s├───────────────────────┼──────────────────────┤%s\n", GRIS_CLAIR, RESET);

    printf("%s│ %-23s │ %-20d │%s\n", VERT, BOIS_ICON " Bois", ressources->bois, RESET);
    printf("%s│ %-24s │ %-20d │%s\n", GRIS_FONCE, PIERRE_ICON "  Pierre ", ressources->pierre, RESET);
    printf("%s│ %-26s │ %-20d │%s\n", GRIS_CLAIR, FER_ICON "  Fer", ressources->fer, RESET);
    printf("%s│ %-24s │ %-20d │%s\n", JAUNE_OR, OR_ICON "  Or", ressources->or, RESET);
    printf("%s│ %-23s │ %-20d │%s\n", CYAN, DIAMANT_ICON " Diamant", ressources->diamant, RESET);
    printf("%s│ %-23s │ %-20d │%s\n", ROUGE, ARGENT_ICON " Argent", ressources->argent, RESET);

    printf("%s└───────────────────────┴──────────────────────┘%s\n", GRIS_CLAIR, RESET);
}

// Fonction principale
int main()
{
    // Récupérer la file de messages
    int msgid = msgget(MESSAGE_QUEUE_KEY, 0666);
    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    struct message msg;

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
        // Vérifier si un message de fin a été reçu
        if (msgrcv(msgid, &msg, sizeof(msg.signal), 1, IPC_NOWAIT) != -1)
        {
            if (msg.signal == 1)
            {
                printf("Signal de fin reçu. Fermeture du programme d'affichage...\n");
                // Suppression de la file de messages
                msgctl(msgid, IPC_RMID, NULL);
                printf("File de messages supprimée.\n");
                // Supprimer le segment de mémoire partagée et la file de messages
                if (shmdt(ressources) == -1)
                {
                    perror("shmdt");
                }
                if (shmctl(shmid, IPC_RMID, NULL) == -1)
                {
                    perror("shmctl");
                }
                printf("Segment de mémoire supprimé.\n");
                break;
            }
        }

        sem_wait(&ressources->semaphore);
        print_header();
        print_table(ressources);
        sem_post(&ressources->semaphore);

        //Attendre 0.5s avant de rafraîchir l'affichage
        usleep(500000);
    }

    return 0;
}