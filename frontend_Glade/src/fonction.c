#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fonction.h"

// ============================================
// FONCTIONS POUR UTILISATEURS
// ============================================
void ajouter(char fullname[], char username[], char password[], int role) {
    FILE *f = fopen("utilisateurs.txt", "a");
    if (f != NULL) {
        fprintf(f, "%s %s %s %d\n", fullname, username, password, role);
        fclose(f);
    }
}

int verifier(char username[], char password[], int *role) {
    FILE *f = fopen("utilisateurs.txt", "r");
    if (f == NULL) {
        return 0; // fichier n'existe pas
    }
    
    char fullnom[50], user[50], pass[50];
    int role_lu;
    
    while (fscanf(f, "%s %s %s %d", fullnom, user, pass, &role_lu) != EOF) {
        if (strcmp(user, username) == 0 && strcmp(pass, password) == 0) {
            fclose(f);
            *role = role_lu;
            return 1; // authentification réussie
        }
    }
    
    fclose(f);
    return 0; // échec
}
// ============================================
// FONCTIONS POUR RÉSERVATIONS
// ============================================

void ajouter_reservation(ReservationEquipement *reservation) {
    FILE *f = fopen("reservations.txt", "a");
    if (f != NULL) {
        fprintf(f, "%s;%s;%s;%d;%s;%02d/%02d/%04d;%s\n",
                reservation->entraineur,
                reservation->type_equipement,
                reservation->nom_equipement,
                reservation->quantite,
                reservation->salle,
                reservation->jour, reservation->mois, reservation->annee,
                reservation->periode);
        fclose(f);
    }
}
