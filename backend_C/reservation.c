#include "reservation.h"
int ajouter_reservation(char * filename, Reservation r)
{
	FILE * f=fopen(filename,"a");
	if(f!=NULL)
	{
		fprintf(f,"%d|%s|%s|%s|%d|%s\n", r.id, r.cours, r.salle, r.equipement, r.quantite, r.date);
		fclose(f);
		return 1;
	}
	else return 0;
}
int modifier_reservation(char * filename, int id, Reservation nouv)
{
	int tr=0;
	Reservation r;
	FILE * f=fopen(filename,"r");
	FILE * f2=fopen("nouv.txt","w");
	if(f!= NULL && f2!= NULL)
	{
		while(fscanf(f,"%d|%49[^|]|%29[^|]|%49[^|]|%d|%10[^\n]", &r.id, r.cours, r.salle, r.equipement,&r.quantite, r.date)!=EOF)
		{
			if(r.id==id)
			{
				fprintf(f2,"%d|%s|%s|%s|%d|%s\n", nouv.id, nouv.cours, nouv.salle, nouv.equipement, nouv.quantite, nouv.date);
				tr=1;
			}
			else
				fprintf(f2,"%d|%s|%s|%s|%d|%s\n", r.id, r.cours, r.salle, r.equipement, r.quantite, r.date);
		}
	}
	fclose(f);
	fclose(f2);
	remove(filename);
	rename("nouv.txt",filename);
	return tr;
}
int supprimer_reservation(char * filename, int id)
{
	int tr=0;
	Reservation r;
	FILE * f=fopen(filename,"r");
	FILE * f2=fopen("nouv.txt","w");
	if(f!=NULL && f2!=NULL)
	{
		while(fscanf(f,"%d|%49[^|]|%29[^|]|%49[^|]|%d|%10[^\n]",&r.id, r.cours, r.salle, r.equipement, &r.quantite, r.date)!=EOF)
		{
			if(r.id==id)
				tr=1;
			else
				fprintf(f2,"%d|%s|%s|%s|%d|%s\n", r.id, r.cours, r.salle, r.equipement, r.quantite, r.date);
		}
	}
	fclose(f);
	fclose(f2);
	remove(filename);
	rename("nouv.txt", filename);
	return tr;
}
Reservation chercher_reservation(char * filename, int id)
{
	Reservation r;
	int tr=0;
	FILE * f=fopen(filename,"r");
	if(f!=NULL)
	{
		while(tr==0 && fscanf(f,"%d|%49[^|]|%29[^|]|%49[^|]|%d|%10[^\n]", &r.id, r.cours, r.salle, r.equipement, &r.quantite, r.date)!=EOF)
		{
			if(r.id==id)
				tr=1;
		}
	}
	fclose(f);
	if(tr==0)
		r.id=-1;
	return r;
}
