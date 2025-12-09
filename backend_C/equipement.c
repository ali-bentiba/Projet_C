#include "equipement.h"
#include <string.h>
int ajouter_equipement(char * filename, Equipement e)
{
	FILE * f=fopen(filename,"a");
	if(f!=NULL)
	{
		fprintf(f,"%d|%s|%s|%s|%d|%s\n", e.id, e.nom, e.type, e.salle, e.statut, e.date_achat);
	fclose(f);
	return 1;
	}
	else return 0;
}
int modifier_equipement(char * filename, int id, Equipement nouv)
{
	int tr=0;
	Equipement e;
	FILE * f=fopen(filename, "r");
	FILE * f2=fopen("nouv.txt","w");
	if(f!= NULL && f2!= NULL)
	{
		while(fscanf(f,"%d|%49[^|]|%29[^|]|%29[^|]|%d|%10[^\n]", &e.id, e.nom, e.type, e.salle, &e.statut, e.date_achat) !=EOF)
		{
			if(e.id==id)
			{
				fprintf(f2,"%d|%s|%s|%s|%d|%s\n", nouv.id, nouv.nom, nouv.type, nouv.salle, nouv.statut, nouv.date_achat);
				tr=1;
			}
			else
				fprintf(f2,"%d|%s|%s|%s|%d|%s\n", e.id, e.nom, e.type, e.salle, e.statut, e.date_achat);
		}
	}
	fclose(f);
	fclose(f2);
	remove(filename);
	rename("nouv.txt",filename);
	return tr;
}
int supprimer_equipement(char *  filename, int id)
{
	int tr=0;
	Equipement e;
	FILE * f=fopen(filename, "r");
	FILE * f2=fopen("nouv.txt","w");
	if(f!=NULL && f2!=NULL)
	{
		while(fscanf(f,"%d|%49[^|]|%29[^|]|%29[^|]|%d|%10[^\n]",&e.id, e.nom, e.type, e.salle, e.statut, e.date_achat)!=EOF)
		{
			if(e.id==id)
				tr=1;
			else
				fprintf(f2,"%d|%s|%s|%s|%d|%s\n",e.id, e.nom, e.type, e.salle, e.statut, e.date_achat);
		}
	}
	fclose(f);
	fclose(f2);
	remove(filename);
	rename("nouv.txt",filename);
	return tr;
}
Equipement chercher_equipement(char * filename, int id)
{
	Equipement e;
	int tr = 0;
	FILE * f = fopen(filename,"r");
	if(f!=NULL)
	{
		while(tr==0 && fscanf(f,"%d|%49[^|]|%29[^|]|%29[^|]|%d|%10[^\n]",&e.id, e.nom, e.type, e.salle, &e.statut, e.date_achat)!= EOF)
		{
			if(e.id==id)
				tr=1;
		}
	}
	fclose(f);
	if(tr==0)
		e.id=-1;
return e;
}
