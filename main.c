#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
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

void afficherBarreChargement(int duree)
{
    int largeurBarre = 30;                         // Nombre de segments de la barre
    int interval = duree * 1000000 / largeurBarre; // Durée par segment (en microsecondes)

    printf("[");
    for (int i = 0; i < largeurBarre; i++)
    {
        usleep(interval); // Pause pour simuler le chargement
        printf("#");
        fflush(stdout); // Force l'affichage immédiat
    }
    printf("] Terminé !\n");
}

CoutFabrication getCoutFabrication(OutilType type, int niveau)
{
    CoutFabrication cout = {0, 0, 0, 0, 0};
    if (type == PIOCHE)
    {
        switch (niveau)
        {
        case 1: // Bois
            cout.bois = 20;
            break;
        case 2: // Pierre
            cout.bois = 20;
            cout.pierre = 30;
            break;
        case 3: // Fer
            cout.bois = 30;
            cout.pierre = 50;
            cout.fer = 20;
            break;
        case 4: // Or
            cout.bois = 40;
            cout.pierre = 60;
            cout.fer = 30;
            cout.or = 20;
            break;
        case 5: // Diamant
            cout.bois = 50;
            cout.pierre = 80;
            cout.fer = 40;
            cout.diamant = 20;
            break;
        }
    }
    else if (type == HACHE)
    {
        // Même logique pour la hache (adapter les coûts si nécessaire)
        switch (niveau)
        {
        case 1:
            cout.bois = 20;
            break;
        case 2:
            cout.bois = 20;
            cout.pierre = 30;
            break;
        case 3:
            cout.bois = 30;
            cout.pierre = 50;
            cout.fer = 20;
            break;
        case 4:
            cout.bois = 40;
            cout.pierre = 60;
            cout.fer = 30;
            cout.or = 20;
            break;
        case 5:
            cout.bois = 50;
            cout.pierre = 80;
            cout.fer = 40;
            cout.diamant = 20;
            break;
        }
    }
    return cout;
}

int verifierEtConsommerRessources(Ressources *ressources, CoutFabrication cout)
{
    if (ressources->bois < cout.bois || ressources->pierre < cout.pierre ||
        ressources->fer < cout.fer || ressources->or < cout.or || ressources->diamant < cout.diamant)
    {
        printf("Ressources insuffisantes !\n");
        // Afficher les ressources manquantes
        printf("Il vous manque :\n");
        if (ressources->bois < cout.bois)
        {
            printf("Bois : %d\n", cout.bois - ressources->bois);
        }
        if (ressources->pierre < cout.pierre)
        {
            printf("Pierre : %d\n", cout.pierre - ressources->pierre);
        }
        if (ressources->fer < cout.fer)
        {
            printf("Fer : %d\n", cout.fer - ressources->fer);
        }
        if (ressources->or < cout.or)
        {
            printf("Or : %d\n", cout.or -ressources->or);
        }
        if (ressources->diamant < cout.diamant)
        {
            printf("Diamant : %d\n", cout.diamant - ressources->diamant);
        }
        return 0;
    }

    // Consommer les ressources
    ressources->bois -= cout.bois;
    ressources->pierre -= cout.pierre;
    ressources->fer -= cout.fer;
    ressources->or -= cout.or ;
    ressources->diamant -= cout.diamant;
    return 1;
}

void recupererRessources(Joueur *joueur, Ressources *ressources)
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
            // printf("Vous coupez du bois.\n");
            recolterRessource(joueur, ressources, BOIS);
            break;
        case 2:
            // printf("Vous minez de la pierre.\n");
            recolterRessource(joueur, ressources, PIERRE);
            break;
        case 3:
            // printf("Vous minez du fer.\n");
            recolterRessource(joueur, ressources, FER);
            break;
        case 4:
            // printf("Vous minez de l'or.\n");
            recolterRessource(joueur, ressources, OR);
            break;
        case 5:
            // printf("Vous minez du diamant.\n");
            recolterRessource(joueur, ressources, DIAMANT);
            break;
        case 0:
            printf("Vous sortez du Menu Ressources !\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
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

void fabriquerOutils(Joueur *joueur, Ressources *ressources)
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
            fabriquerPioche(joueur, ressources);
            break;
        case 2:
            fabriquerHache(joueur, ressources);
            break;
        case 0:
            printf("Vous sortez du Menu Outils !\n");
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
    }
}

void fabriquerPioche(Joueur *joueur, Ressources *ressources)
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
            if (joueur->niveauPioche > 0)
            {
                printf("Vous avez déjà une pioche en bois !\n");
                break;
            }
            CoutFabrication coutPioche1 = getCoutFabrication(PIOCHE, 1);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutPioche1))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauPioche = 1;
                afficherBarreChargement(10); // Affiche la barre de progression
                printf("Pioche en bois obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une pioche en bois !\n");
            }
            break;
        case 2:
            if (joueur->niveauPioche > 1)
            {
                printf("Vous avez déjà une pioche en pierre !\n");
                break;
            }
            CoutFabrication coutPioche2 = getCoutFabrication(PIOCHE, 2);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutPioche2))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauPioche = 2;
                afficherBarreChargement(20); // Affiche la barre de progression
                printf("Pioche en pierre obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une pioche en pierre !\n");
            }
            break;
        case 3:
            if (joueur->niveauPioche > 2)
            {
                printf("Vous avez déjà une pioche en fer !\n");
                break;
            }
            CoutFabrication coutPioche3 = getCoutFabrication(PIOCHE, 3);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutPioche3))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauPioche = 3;
                afficherBarreChargement(30); // Affiche la barre de progression
                printf("Pioche en fer obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une pioche en fer !\n");
            }
            break;
        case 4:
            if (joueur->niveauPioche > 3)
            {
                printf("Vous avez déjà une pioche en or !\n");
                break;
            }
            CoutFabrication coutPioche4 = getCoutFabrication(PIOCHE, 4);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutPioche4))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauPioche = 4;
                afficherBarreChargement(40); // Affiche la barre de progression
                printf("Pioche en or obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une pioche en or !\n");
            }
            break;
        case 5:
            if (joueur->niveauPioche > 4)
            {
                printf("Vous avez déjà une pioche en diamant !\n");
                break;
            }
            CoutFabrication coutPioche5 = getCoutFabrication(PIOCHE, 5);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutPioche5))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauPioche = 5;
                afficherBarreChargement(50); // Affiche la barre de progression
                printf("Pioche en diamant obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une pioche en diamant !\n");
            }
            break;
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

void fabriquerHache(Joueur *joueur, Ressources *ressources)
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
            if (joueur->niveauHache > 0)
            {
                printf("Vous avez déjà une hache en bois !\n");
                break;
            }
            CoutFabrication coutHache1 = getCoutFabrication(HACHE, 1);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutHache1))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauHache = 1;
                afficherBarreChargement(10); // Affiche la barre de progression
                printf("Hache en bois obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une hahe en bois !\n");
            }
            break;
        case 2:
            if (joueur->niveauHache > 1)
            {
                printf("Vous avez déjà une hache en pierre !\n");
                break;
            }
            CoutFabrication coutHache2 = getCoutFabrication(HACHE, 2);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutHache2))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauHache = 2;
                afficherBarreChargement(20); // Affiche la barre de progression
                printf("Hache en pierre obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une hache en pierre !\n");
            }
            break;
        case 3:
            if (joueur->niveauHache > 2)
            {
                printf("Vous avez déjà une hache en fer !\n");
                break;
            }
            CoutFabrication coutHache3 = getCoutFabrication(HACHE, 3);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutHache3))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauHache = 3;
                afficherBarreChargement(30); // Affiche la barre de progression
                printf("Hache en fer obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une hache en fer !\n");
            }
            break;
        case 4:
            if (joueur->niveauHache > 3)
            {
                printf("Vous avez déjà une hache en or !\n");
                break;
            }
            CoutFabrication coutHache4 = getCoutFabrication(HACHE, 4);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutHache4))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauHache = 4;
                afficherBarreChargement(40); // Affiche la barre de progression
                printf("Hache en or obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une hache en or !\n");
            }
            break;
        case 5:
            if (joueur->niveauHache > 4)
            {
                printf("Vous avez déjà une hache en diamant !\n");
                break;
            }
            CoutFabrication coutHache5 = getCoutFabrication(HACHE, 5);
            sem_wait(&ressources->semaphore);
            if (verifierEtConsommerRessources(ressources, coutHache5))
            {
                sem_post(&ressources->semaphore);
                joueur->niveauHache = 5;
                afficherBarreChargement(50); // Affiche la barre de progression
                printf("Hache en diamant obtenu !\n");
            }
            else
            {
                sem_post(&ressources->semaphore);
                printf("Vous n'avez pas assez de ressources pour fabriquer une hache en diamant !\n");
            }
            break;
        case 0:
            return;
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
        }
        if (choixMateriau != 0)
        {
            printf("\nAppuyez sur Entrée pour continuer ...");
            getchar();
            getchar(); // Pause avant le rafraîchissement
        }
    }
}

// Fonction pour obtenir le nom d'une ressource
const char *getNomRessource(RessourceType type)
{
    const char *noms[] = {"Bois", "Pierre", "Fer", "Or", "Diamant"};
    return noms[type];
}

void recolterRessource(Joueur *joueur, Ressources *ressources, RessourceType type)
{
    int quantite = 10; // Quantité par défaut

    if (type == BOIS)
    {
        if (joueur->niveauHache > 0)
        {
            quantite = (1 << joueur->niveauHache); // 2^niveauHache
        }
    }
    else
    {
        if (joueur->niveauPioche == 0)
        {
            printf("Vous avez besoin d'une pioche pour miner %s !\n", getNomRessource(type));
            getchar();
            getchar();
            return;
        }

        switch (type)
        {
        case PIERRE:
            if (joueur->niveauPioche >= 1)
            {
                quantite = (1 << (joueur->niveauPioche - 1)); // 2^niveauPioche
            }else{
                printf("Vous avez besoin d'une pioche en bois pour miner %s !\n", getNomRessource(type));
                getchar();
                getchar();
            }
            break;
        case FER:
            if (joueur->niveauPioche >= 2)
            {
                quantite = (1 << (joueur->niveauPioche - 2)); // 2^(niveauPioche-1)
            }else{
                printf("Vous avez besoin d'une pioche en pierre pour miner %s !\n", getNomRessource(type));
                getchar();
                getchar();
            }
            break;
        case OR:
            if (joueur->niveauPioche >= 3)
            {
                quantite = (1 << (joueur->niveauPioche - 3)); // 2^(niveauPioche-2)
            }else{
                printf("Vous avez besoin d'une pioche en fer pour miner %s !\n", getNomRessource(type));
                getchar();
                getchar();
            }
            break;
        case DIAMANT:
            if (joueur->niveauPioche >= 4)
            {
                quantite = (1 << (joueur->niveauPioche - 4)); // 2^(niveauPioche-3)
            }else{
                printf("Vous avez besoin d'une en or pioche pour miner %s !\n", getNomRessource(type));
                getchar();
                getchar();
            }
            break;
        default:
            printf("Type de ressource inconnu !\n");
            return;
        }
    }

    printf("Récolte en cours : %s...\n", getNomRessource(type));
    afficherBarreChargement(joueur->recolteTemps); // Affiche la barre de progression

    // Protection de l'accès à la mémoire partagée
    sem_wait(&ressources->semaphore);
    switch (type)
    {
    case BOIS:
        ressources->bois += quantite;
        break;
    case PIERRE:
        ressources->pierre += quantite;
        break;
    case FER:
        ressources->fer += quantite;
        break;
    case OR:
        ressources->or += quantite;
        break;
    case DIAMANT:
        ressources->diamant += quantite;
        break;
    default:
        printf("Type de ressource inconnu !\n");
        break;
    }
    sem_post(&ressources->semaphore);

    printf("Vous avez récolté %d unité(s) de %s.\n", quantite, getNomRessource(type));
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

    Joueur joueur = {0, 0, 1}; // Pas de hache, pas de pioche, temps de récolte de 5 secondes

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
    // ressources->bois = 0;
    // ressources->pierre = 0;
    // ressources->fer = 0;
    // ressources->or = 0;
    // ressources->diamant = 0;
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
            fabriquerOutils(&joueur, ressources);
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
