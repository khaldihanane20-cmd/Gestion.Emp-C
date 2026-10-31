
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define T 50
#define FICH "emp.txt"

typedef struct {
    int mat;
    char nom[T];
    char prenom[T];
    char poste[T];
    char service[T];
    float salaire;
} Emp;


int lireLigne(char *s, int taille)
{
    if (fgets(s, taille, stdin) == NULL)
        return 0;

    s[strcspn(s, "\n")] = '\0';

    return 1;
}


int estVide(char *s)
{
    int i = 0;

    while (s[i] != '\0') {
        if (!isspace((unsigned char)s[i]))
            return 0;
        i++;
    }

    return 1;
}


int lireEntier(char *msg, int min, int max)
{
    int n;

    while (1) {
        printf("%s", msg);

        if (scanf("%d", &n) != 1) {
            printf("Erreur de saisie !\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (n >= min && n <= max)
            return n;

        printf("Valeur incorrecte !\n");
    }
}


float lireSalaire(char *msg)
{
    float s;

    while (1) {
        printf("%s", msg);

        if (scanf("%f", &s) != 1) {
            printf("Salaire invalide !\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (s >= 0)
            return s;

        printf("Le salaire doit etre positif !\n");
    }
}


void lireTexte(char *msg, char *dest, int taille)
{
    do {
        printf("%s", msg);
        fgets(dest, taille, stdin);

        dest[strcspn(dest, "\n")] = '\0';

        if (estVide(dest))
            printf("Ce champ est obligatoire !\n");

    } while (estVide(dest));
}


int confirmer(char *msg)
{
    char rep;

    printf("%s (o/n) : ", msg);
    scanf(" %c", &rep);
    while (getchar() != '\n');

    return (rep == 'o' || rep == 'O');
}


int rechercher(Emp t[], int nb, int mat)
{
    for (int i = 0; i < nb; i++) {
        if (t[i].mat == mat)
            return i;
    }

    return -1;
}


void afficherTitre()
{
    printf("\nMatricule  Nom       Prenom    Poste       Service     Salaire\n");
    printf("-------------------------------------------------------------\n");
}


void afficherUn(Emp e)
{
    printf("%-10d %-10s %-10s %-12s %-12s %.2f\n",
           e.mat, e.nom, e.prenom, e.poste, e.service, e.salaire);
}


void afficherTous(Emp t[], int nb)
{
    if (nb == 0) {
        printf("Aucun employe enregistre.\n");
        return;
    }

    afficherTitre();

    for (int i = 0; i < nb; i++) {
        afficherUn(t[i]);
    }

    printf("\nNombre total : %d employe(s).\n", nb);
}



void ajouter(Emp t[], int *nb, int *modifie)
{
    Emp e;

    if (*nb >= MAX) {
        printf("Le tableau est plein !\n");
        return;
    }

    printf("\n--- Ajouter un employe ---\n");

    e.mat = lireEntier("Matricule : ", 1, 999999);

    if (rechercher(t, *nb, e.mat) != -1) {
        printf("Ce matricule existe deja !\n");
        return;
    }

    lireTexte("Nom : ", e.nom, T);
    lireTexte("Prenom : ", e.prenom, T);
    lireTexte("Poste : ", e.poste, T);
    lireTexte("Service : ", e.service, T);

    e.salaire = lireSalaire("Salaire : ");

    t[*nb] = e;
    (*nb)++;

    *modifie = 1;

    printf("Employe ajoute avec succes !\n");
}



void rechercherEmp(Emp t[], int nb)
{
    int mat, i;

    printf("\n--- Recherche d'un employe ---\n");

    mat = lireEntier("Matricule : ", 1, 999999);

    i = rechercher(t, nb, mat);

    if (i == -1) {
        printf("Employe introuvable !\n");
    }
    else {
        afficherTitre();
        afficherUn(t[i]);
    }
}



void modifier(Emp t[], int nb, int *modifie)
{
    int mat, i;

    printf("\n--- Modifier un employe ---\n");

    mat = lireEntier("Matricule : ", 1, 999999);

    i = rechercher(t, nb, mat);

    if (i == -1) {
        printf("Employe introuvable !\n");
        return;
    }

    printf("\nNouvelles informations :\n");

    lireTexte("Nouveau nom : ", t[i].nom, T);
    lireTexte("Nouveau prenom : ", t[i].prenom, T);
    lireTexte("Nouveau poste : ", t[i].poste, T);
    lireTexte("Nouveau service : ", t[i].service, T);

    t[i].salaire = lireSalaire("Nouveau salaire : ");

    *modifie = 1;

    printf("Modification effectuee avec succes !\n");
}



void supprimer(Emp t[], int *nb, int *modifie)
{
    int mat, i;

    printf("\n--- Suppression d'un employe ---\n");

    mat = lireEntier("Matricule a supprimer : ", 1, 999999);
    i = rechercher(t, *nb, mat);

    if (i == -1) {
        printf("Employe introuvable !\n");
        return;
    }

    afficherTitre();
    afficherUn(t[i]);

    if (!confirmer("Confirmer la suppression ?")) {
        printf("Suppression annulee.\n");
        return;
    }

    for (int j = i; j < *nb - 1; j++) {
        t[j] = t[j + 1];
    }

    (*nb)--;
    *modifie = 1;

    printf("Employe supprime avec succes !\n");
}



int sauvegarder(Emp t[], int nb)
{
    FILE *f = fopen(FICH, "w");

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier !\n");
        return 0;
    }

    for (int i = 0; i < nb; i++) {
        fprintf(f, "%d;%s;%s;%s;%s;%.2f\n",
                t[i].mat, t[i].nom, t[i].prenom,
                t[i].poste, t[i].service, t[i].salaire);
    }

    fclose(f);

    printf("Sauvegarde effectuee !\n");
    return 1;
}

/* Charger les employes depuis le fichier */
int charger(Emp t[], int *nb)
{
    FILE *f = fopen(FICH, "r");
    Emp e;

    *nb = 0;

    if (f == NULL) {
        return 0;
    }

    while (*nb < MAX &&
           fscanf(f, "%d;%49[^;];%49[^;];%49[^;];%49[^;];%f",
                  &e.mat, e.nom, e.prenom,
                  e.poste, e.service, &e.salaire) == 6) {

        t[*nb] = e;
        (*nb)++;
    }

    fclose(f);
    return 1;
}



void afficherMenu()
{
    printf("\n==============================\n");
    printf("     GESTION DES EMPLOYES\n");
    printf("==============================\n");
    printf("1. Ajouter un employe\n");
    printf("2. Afficher tous les employes\n");
    printf("3. Rechercher un employe\n");
    printf("4. Modifier un employe\n");
    printf("5. Supprimer un employe\n");
    printf("6. Enregistrer dans le fichier\n");
    printf("7. Quitter\n");
    printf("==============================\n");
}

int main()
{
    Emp t[MAX];
    int nb = 0;
    int modifie = 0;
    int choix;

    if (charger(t, &nb))
        printf("%d employe(s) charge(s).\n", nb);
    else
        printf("Aucun fichier trouve. Liste vide.\n");

    do {
        afficherMenu();
        choix = lireEntier("Votre choix : ", 1, 7);

        switch (choix) {

            case 1:
                ajouter(t, &nb, &modifie);
                break;

            case 2:
                afficherTous(t, nb);
                break;

            case 3:
                rechercherEmp(t, nb);
                break;

            case 4:
                modifier(t, nb, &modifie);
                break;

            case 5:
                supprimer(t, &nb, &modifie);
                break;

            case 6:
                if (sauvegarder(t, nb)) {
                    modifie = 0;
                    printf("Donnees enregistrees !\n");
                }
                break;

            case 7:
                if (modifie) {
                    if (confirmer("Sauvegarder avant de quitter ?")) {
                        if (sauvegarder(t, nb))
                            printf("Donnees enregistrees !\n");
                    }
                }

                printf("Au revoir !\n");
                break;
        }

    } while (choix != 7);

    return 0;
}

