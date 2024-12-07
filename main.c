#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include "pthread.h"
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include "main.h"

// Fonction d'un ouvrier
void *thread_ouvrier(void *arg)
{
    Ressources *ressources = (Ressources *)arg;

    while (1)
    {
        // Simuler le travail (par exemple, couper du bois)
        sleep(1); // Pause de 2 secondes pour simuler le temps de travail

        // Accès exclusif à la mémoire partagée
        sem_wait(&ressources->semaphore);
        ressources->bois += 10; // Ajouter 10 unités de bois
        // printf("Ouvrier a ajouté 10 unités de bois. Total bois : %d\n", ressources->bois);
        sem_post(&ressources->semaphore);
    }

    return NULL;
}

void *ouvrirTerminalAffichage()
{
    // system("xterm -e ./affichage &");
    return NULL;
}

void clearScreen()
{
// Efface l'écran en fonction de l'OS
#ifdef _WIN32
    system("cls"); // Commande Windows
#else
    system("clear"); // Commande Linux/Mac
#endif
}

void afficherBarreChargement(int duree) {
    int largeurBarre = 30; // Nombre de segments de la barre
    int interval = duree * 1000000 / largeurBarre; // Durée par segment (en microsecondes)

    printf("[");
    for (int i = 0; i < largeurBarre; i++) {
        usleep(interval); // Pause pour simuler le chargement
        printf("#");
        fflush(stdout); // Force l'affichage immédiat
    }
    printf("] Terminé !\n");
}

void recupererRessources(Joueur *joueur, int *ressources)
{
    int choixRessource;
    while (1)
    {
        clearScreen(); // Rafraîchit l'écran
        printf("\n=== Menu Ressources ===\n");
        printf("1 - Couper du bois\n");
        printf("2 - Miner de la pierre\n");
        printf("3 - Miner du fer\n");
        printf("4 - Miner de l'or\n");
        printf("5 - Miner du diamant\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choixRessource);

        switch (choixRessource)
        {
        case 1:
            printf("Vous coupez du bois.\n");
            recolterRessource(joueur, ressources, BOIS);
            break;
        case 2:
            printf("Vous minez de la pierre.\n");
            break;
        case 3:
            printf("Vous minez du fer.\n");
            break;
        case 4:
            printf("Vous minez de l'or.\n");
            break;
        case 5:
            printf("Vous minez du diamant.\n");
            break;
        case 0:
            printf("Vous sortez du Menu Ressources !\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar(); // Pause avant le rafraîchissement
    }
}

void vendreRessources()
{
    int choixVendre;
    while (1)
    {
        clearScreen(); // Rafraîchit l'écran
        printf("\n=== Menu Ventes ===\n");
        printf("1 - Vendre du bois\n");
        printf("2 - Vendre de la pierre\n");
        printf("3 - Vendre du fer\n");
        printf("4 - Vendre de l'or\n");
        printf("5 - Vendre du diamant\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choixVendre);

        switch (choixVendre)
        {
        case 1:
            vendreQuantite(BOIS);
            break;
        case 2:
            vendreQuantite(PIERRE);
            break;
        case 3:
            vendreQuantite(FER);
            break;
        case 4:
            vendreQuantite(OR);
            break;
        case 5:
            vendreQuantite(DIAMANT);
            break;
        case 0:
            printf("Vous sortez du Menu Ventes !\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar(); // Pause avant le rafraîchissement
    }
}

void gererOuvriers()
{
    clearScreen();
    printf("Vous avez choisi de gérer les ouvriers.\n");
    int choixOuvrier;
    while (1)
    {
        clearScreen(); // Rafraîchit l'écran
        printf("\n=== Menu Ouvrier ===\n");
        printf("1 - Acheter un ouvrier\n");
        printf("2 - Affecter un ouvrier\n");
        printf("3 - Améliorer un ouvrier\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choixOuvrier);

        switch (choixOuvrier)
        {
        case 1:
            printf("Vous achetez un ouvrier.\n");
            break;
        case 2:
            affecterOuvriers();
            break;
        case 3:
            ameliorerOuvriers();
            break;
        case 0:
            printf("Vous sortez du Menu Ouvrier !\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar(); // Pause avant le rafraîchissement
    }
}

void fabriquerOutils()
{
    int choixOutils;
    while (1)
    {
        clearScreen(); // Rafraîchit l'écran
        printf("\n=== Menu Outils ===\n");
        printf("1 - Fabriquer une pioche\n");
        printf("2 - Fabriquer une hache\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choixOutils);

        switch (choixOutils)
        {
        case 1:
            fabriquerPioche();
            break;
        case 2:
            fabriquerHache();
            break;
        case 0:
            printf("Vous sortez du Menu Outils !\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar(); // Pause avant le rafraîchissement
    }
}

void ameliorerOuvriers()
{
    printf("Liste des ouvriers");
}

void affecterOuvriers()
{
    clearScreen();
    printf("Vous avez choisi d'affecter des ouvriers.\n");
    int choixAffectation;
    while (1)
    {
        clearScreen(); // Rafraîchit l'écran
        printf("\n=== Menu Affectation des ouvriers ===\n");
        printf("1 - Couper du bois\n");
        printf("2 - Miner de la pierre\n");
        printf("3 - Miner du fer\n");
        printf("4 - Miner de l'or\n");
        printf("5 - Miner du diamant\n");
        printf("6 - Affecter au four\n");
        printf("7 - Vendeur de stockage\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choixAffectation);

        switch (choixAffectation)
        {
        case 1:
            printf("Vous affectez un ouvrier pour couper du bois.\n");
            break;
        case 2:
            printf("Vous affectez un ouvrier pour miner la pierre.\n");
            break;
        case 3:
            printf("Vous affectez un ouvrier pour miner du fer.\n");
            break;
        case 4:
            printf("Vous affectez un ouvrier pour miner de l'or.\n");
            break;
        case 5:
            printf("Vous affectez un ouvrier pour miner du diamant.\n");
            break;
        case 6:
            printf("Vous affectez un ouvrier au four.\n");
            break;
        case 7:
            printf("Vous affectez un ouvrier vendeur de stockage.\n");
            break;
        case 0:
            printf("Vous sortez du Menu Ouvrier !\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar(); // Pause avant le rafraîchissement
    }
}

void vendreQuantite(int ressource)
{
    const char *nomsRessources[] = {"bois", "pierre", "fer", "or", "diamant"};
    int choixQuantite;

    while (1)
    {
        clearScreen(); // Rafraîchit l'écran
        printf("\n=== Menu Vente : %s ===\n", nomsRessources[ressource]);
        printf("1 - Tout vendre\n");
        printf("2 - Vendre une quantité personnalisée\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choixQuantite);

        switch (choixQuantite)
        {
        case 1:
            printf("Vous vendez tout le %s disponible.\n", nomsRessources[ressource]);
            // Logique pour vendre tout
            return;
        case 2:
        {
            int quantite;
            printf("Entrez la quantité de %s à vendre : ", nomsRessources[ressource]);
            scanf("%d", &quantite);
            printf("Vous vendez %d unités de %s.\n", quantite, nomsRessources[ressource]);
            // Logique pour vendre une quantité personnalisée
            return;
        }
        case 0:
            printf("Retour au menu précédent.\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar(); // Pause avant le rafraîchissement
    }
}

void fabriquerPioche()
{
    int choixMateriau;
    while (1)
    {
        clearScreen(); // Rafraîchit l'écran
        printf("\n=== Menu Fabrication : Pioche ===\n");
        printf("1 - Pioche en bois\n");
        printf("2 - Pioche en pierre\n");
        printf("3 - Pioche en fer\n");
        printf("4 - Pioche en or\n");
        printf("5 - Pioche en diamant\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choixMateriau);

        switch (choixMateriau)
        {
        case 1:
            printf("Vous fabriquez une pioche en bois.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 2:
            printf("Vous fabriquez une pioche en pierre.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 3:
            printf("Vous fabriquez une pioche en fer.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 4:
            printf("Vous fabriquez une pioche en or.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 5:
            printf("Vous fabriquez une pioche en diamant.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 0:
            printf("Retour au menu précédent.\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar(); // Pause avant le rafraîchissement
    }
}

void fabriquerHache()
{
    int choixMateriau;
    while (1)
    {
        clearScreen(); // Rafraîchit l'écran
        printf("\n=== Menu Fabrication : Hache ===\n");
        printf("1 - Hache en bois\n");
        printf("2 - Hache en pierre\n");
        printf("3 - Hache en fer\n");
        printf("4 - Hache en or\n");
        printf("5 - Hache en diamant\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choixMateriau);

        switch (choixMateriau)
        {
        case 1:
            printf("Vous fabriquez une hache en bois.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 2:
            printf("Vous fabriquez une hache en pierre.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 3:
            printf("Vous fabriquez une hache en fer.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 4:
            printf("Vous fabriquez une hache en or.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 5:
            printf("Vous fabriquez une hache en diamant.\n");
            // Ajouter la logique pour consommer les ressources nécessaires
            return;
        case 0:
            printf("Retour au menu précédent.\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar(); // Pause avant le rafraîchissement
    }
}

// Fonction pour obtenir le nom d'une ressource
const char *getNomRessource(RessourceType type)
{
    const char *noms[] = {"Bois", "Pierre", "Fer", "Or", "Diamant"};
    return noms[type];
}

void recolterRessource(Joueur *joueur, int *ressources, RessourceType type) {
    int quantite = 1; // Quantité par défaut

    // Ajuster la quantité récoltée selon l'outil
    if (joueur->outil.type == HACHE && type == BOIS) {
        quantite *= (joueur->outil.niveau + 1); // Double la récolte pour chaque niveau
    } else if (joueur->outil.type == PIOCHE && type != BOIS) {
        quantite *= (joueur->outil.niveau + 1); // Double la récolte pour chaque niveau
    }

    printf("Récolte en cours : %s...\n", getNomRessource(type));
    afficherBarreChargement(joueur->recolteTemps); // Affiche la barre de progression
    ressources[type] += quantite;
    printf("Vous avez récolté %d unité(s) de %s. Total : %d\n", quantite, getNomRessource(type), ressources[type]);
}

// Fonction pour fabriquer un outil
void fabriquerOutil(Joueur *joueur, int *ressources, OutilType outilType, int niveau)
{
    const int cout[] = {30, 50, 80, 100}; // Coût pour chaque niveau
    if (ressources[BOIS] < cout[niveau - 1])
    {
        printf("Pas assez de bois pour fabriquer l'outil !\n");
        return;
    }

    ressources[BOIS] -= cout[niveau - 1];
    joueur->outil.type = outilType;
    joueur->outil.niveau = niveau;

    printf("Vous avez fabriqué un %s de niveau %d.\n",
           outilType == HACHE ? "hache" : "pioche",
           niveau);
}

void afficherRessources(Ressources *ressources)
{
    sem_wait(&ressources->semaphore);
    printf("Ressources actuelles :\n");
    printf("Bois : %d\n", ressources->bois);
    printf("Pierre : %d\n", ressources->pierre);
    printf("Fer : %d\n", ressources->fer);
    printf("Or : %d\n", ressources->or);
    printf("Diamant : %d\n", ressources->diamant);
    sem_post(&ressources->semaphore);
    getchar();
    getchar();
}

int main()
{

    Joueur joueur = {{MAIN, 0}, 5}; // Joueur avec main par défaut et 5 secondes par récolte

    // Clé pour la mémoire partagée
    const char *pathname = "README.md";
    key_t key = ftok(pathname, 7171);
    if (key == -1)
    {
        perror("ftok");
        return 1;
    }

    // Création ou récupération du segment de mémoire partagée
    int shmid = shmget(key, sizeof(Ressources), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }
    printf("Shared memory ID: %d\n", shmid);

    // Attachement au segment de mémoire partagée
    Ressources *ressources = (Ressources *)shmat(shmid, NULL, 0);
    if (ressources == (void *)-1)
    {
        perror("shmat");
        return 1;
    }

    // Initialisation des ressources et du sémaphore
    ressources->bois = 0;
    ressources->pierre = 0;
    ressources->fer = 0;
    ressources->or = 0;
    ressources->diamant = 0;
    sem_init(&ressources->semaphore, 1, 1); // Sémaphore partagé entre processus

    // Création d'un thread ouvrier
    /*
    pthread_t ouvrier;
    pthread_create(&ouvrier, NULL, thread_ouvrier, ressources);
    */

    // Lancer un nouveau terminal pour l'affichage
    pthread_t affichageThread;
    pthread_create(&affichageThread, NULL, ouvrirTerminalAffichage, NULL);

    int choix;

    while (1)
    {
        clearScreen();
        printf("\n=== Menu Principal ===\n");
        printf("1 - Récupérer ressources\n");
        printf("2 - Vendre ressources\n");
        printf("3 - Gérer les ouvriers\n");
        printf("4 - Fabriquer des outils\n");
        printf("5 - Afficher Ressources\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            recupererRessources(&joueur, ressources);
            break;
        case 2:
            vendreRessources();
            break;
        case 3:
            gererOuvriers();
            break;
        case 4:
            fabriquerOutils();
            break;
        case 5:
            afficherRessources(ressources);
            break;
        case 0:
            printf("Au revoir !\n");
            // pthread_cancel(ouvrier);
            // pthread_join(ouvrier, NULL);
            sem_destroy(&ressources->semaphore);
            if (shmdt(ressources) == -1)
            {
                perror("shmdt");
            }
            if (shmctl(shmid, IPC_RMID, NULL) == -1)
            {
                perror("shmctl");
            }
            printf("Segment de mémoire supprimé.\n");
            exit(0);
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
    }

    return 0;
}
