#ifndef RESERVATION_H_INCLUDED
#define RESERVATION_H_INCLUDED
#include <stdio.h>
typedef struct {
	int id;
	char cours[50];
	char salle[30];
	char equipement[50];
	int quantite;
	char date[11];
} Reservation;
int ajouter_reservation(char *, Reservation);
int modifier_reservation(char *, int, Reservation);
int supprimer_reservation(char *, int);
Reservation chercher_reservation(char *, int);
#endif
