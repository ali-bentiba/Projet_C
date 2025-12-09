#ifndef FONCTION_H
#define FONCTION_H

typedef struct {
    char entraineur[50];
    char type_equipement[50];
    char nom_equipement[50];
    int quantite;
    char salle[10];
    int jour;
    int mois;
    int annee;
    char periode[50];
} ReservationEquipement;

// Fonctions pour utilisateurs
void ajouter(char fullname[], char username[], char password[], int role);
int verifier(char username[], char password[], int *role);

// Fonctions pour réservations 
void ajouter_reservation(ReservationEquipement *reservation);
#endif
