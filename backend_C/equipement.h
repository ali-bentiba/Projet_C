#ifndef EQUIPEMENT_H_INCLUDED
#define EQUIPEMENT_H_INCLUDED
#include <stdio.h>
typedef struct {
	int id;
	char nom[50];
	char type[30];
	char salle[30];
	int statut;
	char date_achat[11];
} Equipement;
int ajouter_equipement(char *,Equipement);
int modifier_equipement(char *, int, Equipement);
int supprimer_equipement(char *, int);
Equipement chercher_equipement(char *, int);
#endif
