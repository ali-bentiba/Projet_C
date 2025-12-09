#include <stdio.h>
#include <string.h>
#include "equipement.h"
#include "reservation.h"
int main()
{
	FILE *check = fopen("equipements.txt","a");
	if (check) 
	{
        	fclose(check);
        	printf("equipements.txt is ready\n");
    	} 
	else 
	{
        	printf("Warning: Could not create/open equipements.txt\n");
    	}
	check = fopen("reservations.txt", "a");
    	if (check) 
	{
        	fclose(check);
        	printf("reservations.txt is ready\n");
    	} 
	else 
	{
        	printf("Warning: Could not create/open reservations.txt\n");
    	}
	Equipement e1={1,"Tapis roulant","Cardio","Salle A","Libre","25/05/2023"};
	Equipement e2={2,"Halteres","Musculation","Salle B","Reserve","10/03/2023"};
	Equipement e3;
	Reservation r1={1, "Yoga", "Salle A", "Tapis roulant", 2, "15/11/2023"};
	Reservation r2={2, "Musculation", "Salle B", "Halteres", 1, "16/11/2023"};
	Reservation r3;
	ajouter_equipement("equipements.txt", e1);
	ajouter_equipement("equipements.txt", e2);
	Equipement e_modif = {1,"Tapis roulant pro","Cardio","Salle A", "Maintenance", "25/05/2023"};
	modifier_equipement("equipements.txt",1,e_modif);
	e3=chercher_equipement("equipements.txt",2);
	supprimer_equipement("equipements.txt",1);
	ajouter_reservation("reservations.txt", r1);
	ajouter_reservation("reservations.txt", r2);
	Reservation r_modif= {1,"Yoga Avance","Salle A","Tapis roulant",3,"15/11/2023"};
	modifier_reservation("reservations.txt", 1, r_modif);
	r3=chercher_reservation("reservations.txt",2);
	supprimer_reservation("reservations.txt",1);
	return 0;
}
