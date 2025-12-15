#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"
#include <gtk/gtk.h>
enum
{
    EENTRAINEUR,
    ETYPE,
    EEQUIPEMENT,
    EQUANTITE,
    ESALLE,
    EDATE,
    EPERIODE,
    COLUMNS
};

// ============================================
// FONCTIONS POUR UTILISATEURS
// ============================================
void ajouter(char fullname[], char username[], char password[], int role) {
    FILE *f = fopen("utilisateurs.txt", "a");
    if (f != NULL) {
        // Remplacer TOUS les espaces par underscores
        char fullname_mod[100];
        strcpy(fullname_mod, fullname);
        for (int i = 0; fullname_mod[i]; i++) {
            if (fullname_mod[i] == ' ') fullname_mod[i] = '_';
        }
        
        fprintf(f, "%s %s %s %d\n", fullname_mod, username, password, role);
        fclose(f);
        g_print("DEBUG: Utilisateur enregistré: %s %s %s %d\n", 
                fullname_mod, username, password, role);
    }
}
int verifier(char username[], char password[], int *role) {
    FILE *f = fopen("utilisateurs.txt", "r");
    if (f == NULL) {
        g_print("DEBUG: Fichier introuvable\n");
        return 0;
    }
    
    g_print("DEBUG: Recherche '%s'/'%s'\n", username, password);
    
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;
        g_print("DEBUG: Ligne: '%s'\n", ligne);
        
        char full[50], user[50], pass[50];
        int r;
        
        // Essayer plusieurs formats
        int n = sscanf(ligne, "%49s %49s %49s %d", full, user, pass, &r);
        g_print("DEBUG: sscanf a lu %d champs\n", n);
        
        if (n == 4) {
            g_print("DEBUG: Comparaison: user='%s' vs '%s', pass='%s' vs '%s'\n", 
                   user, username, pass, password);
            
            if (strcmp(user, username) == 0 && strcmp(pass, password) == 0) {
                g_print("DEBUG: MATCH! Role=%d\n", r);
                fclose(f);
                *role = r;
                return 1;
            }
        }
    }
    
    fclose(f);
    g_print("DEBUG: Pas de match\n");
    return 0;
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

void afficher_reservations_dans_treeview(GtkTreeView *treeview) {
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeIter iter;
	GtkListStore *store;

    	char entraineur[50];
    	char type[50];
    	char equipement[50];
    	int quantite;
    	char salle[10];
	char date[20];
    	char periode[50];

	// 1. Vérifier si le TreeView a déjà des colonnes
    	store = (GtkListStore *)gtk_tree_view_get_model(treeview);

	if (store==NULL)
	{
		// Créer les colonnes si elles n'existent pas
		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes("entraineur", renderer, "text", EENTRAINEUR, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes("type", renderer, "text", ETYPE, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes("equipement", renderer, "text", EEQUIPEMENT, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes("quantite", renderer, "text", EQUANTITE, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes("salle", renderer, "text", ESALLE, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes("date", renderer, "text", EDATE, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes("periode", renderer, "text", EPERIODE, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);
	}

	// 2. Créer le store (modèle de données)
    	store = gtk_list_store_new (COLUMNS, 
        G_TYPE_STRING,   // EENTRAINEUR
        G_TYPE_STRING,   // ETYPE
        G_TYPE_STRING,   // EEQUIPEMENT
        G_TYPE_STRING,   // EQUANTITE 
        G_TYPE_STRING,   // ESALLE
        G_TYPE_STRING,   // EDATE
        G_TYPE_STRING    // EPERIODE
    	);

	// 3. Lire le fichier
    	FILE *f = fopen("reservations.txt", "r");
	if (f==NULL)
	{
		g_object_unref(store);
		return;
	}

	// 4. Lire chaque ligne
	while(fscanf(f, "%49[^;];%49[^;];%49[^;];%d;%9[^;];%19[^;];%49[^\n]\n", entraineur, type, equipement, &quantite, salle, date, periode) != EOF)
    	{
        	// Convertir quantite en string pour l'affichage
        	char quantite_str[10];
        	sprintf(quantite_str, "%d", quantite);
        
        	// Ajouter au store
        	gtk_list_store_append (store, &iter);
        	gtk_list_store_set (store, &iter, 
            	EENTRAINEUR, entraineur, 
            	ETYPE, type, 
            	EEQUIPEMENT, equipement, 
            	EQUANTITE, quantite_str,   
            	ESALLE, salle, 
            	EDATE, date, 
            	EPERIODE, periode, 
            	-1);
    	}

	fclose(f);

	// 5. Associer le store au TreeView
    	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (store));

    	// 6. Libérer la référence
    	g_object_unref (store);

}

void supprimer_reservation(ReservationEquipement *reservation) {
    ReservationEquipement res;
    FILE *f, *g;
    
    f = fopen("reservations.txt", "r");
    g = fopen("reservations2.txt", "w");
    
    if (f == NULL || g == NULL)
        return;
    else {
        while(fscanf(f, "%49[^;];%49[^;];%49[^;];%d;%9[^;];%d/%d/%d;%49[^\n]\n",
                    res.entraineur, res.type_equipement, res.nom_equipement, &res.quantite,
                    res.salle, &res.jour, &res.mois, &res.annee, res.periode) != EOF) {
            
            if (strcmp(reservation->entraineur, res.entraineur) != 0 ||
                strcmp(reservation->type_equipement, res.type_equipement) != 0 ||
                strcmp(reservation->nom_equipement, res.nom_equipement) != 0 ||
                reservation->quantite != res.quantite ||
                strcmp(reservation->salle, res.salle) != 0 ||
                reservation->jour != res.jour ||
                reservation->mois != res.mois ||
                reservation->annee != res.annee ||
                strcmp(reservation->periode, res.periode) != 0) {
                
                fprintf(g, "%s;%s;%s;%d;%s;%02d/%02d/%04d;%s\n",
                        res.entraineur, res.type_equipement, res.nom_equipement, res.quantite,
                        res.salle, res.jour, res.mois, res.annee, res.periode);
            }
        }
    }
    
    fclose(f);
    fclose(g);
    
    remove("reservations.txt");
    rename("reservations2.txt", "reservations.txt");
}
void modifier_reservation (ReservationEquipement *ancienne, ReservationEquipement *nouvelle) {
    ReservationEquipement res;
    FILE *f, *g;
    
    f = fopen("reservations.txt", "r");
    g = fopen("reservations2.txt", "w");
    
    if (f == NULL || g == NULL)
        return;
    else {
        while(fscanf(f, "%49[^;];%49[^;];%49[^;];%d;%9[^;];%d/%d/%d;%49[^\n]\n",
                    res.entraineur, res.type_equipement, res.nom_equipement, &res.quantite,
                    res.salle, &res.jour, &res.mois, &res.annee, res.periode) != EOF) {
            
            // Vérifier si c'est la ligne à modifier
            if (strcmp(ancienne->entraineur, res.entraineur) == 0 &&
                strcmp(ancienne->type_equipement, res.type_equipement) == 0 &&
                strcmp(ancienne->nom_equipement, res.nom_equipement) == 0 &&
                ancienne->quantite == res.quantite &&
                strcmp(ancienne->salle, res.salle) == 0 &&
                ancienne->jour == res.jour &&
                ancienne->mois == res.mois &&
                ancienne->annee == res.annee &&
                strcmp(ancienne->periode, res.periode) == 0) {
                
                // Écrire la NOUVELLE version
                fprintf(g, "%s;%s;%s;%d;%s;%02d/%02d/%04d;%s\n",
                        nouvelle->entraineur, nouvelle->type_equipement, nouvelle->nom_equipement, nouvelle->quantite,
                        nouvelle->salle, nouvelle->jour, nouvelle->mois, nouvelle->annee, nouvelle->periode);
            }
            else {
                // Écrire l'ancienne ligne inchangée
                fprintf(g, "%s;%s;%s;%d;%s;%02d/%02d/%04d;%s\n",
                        res.entraineur, res.type_equipement, res.nom_equipement, res.quantite,
                        res.salle, res.jour, res.mois, res.annee, res.periode);
            }
        }
    }
    
    fclose(f);
    fclose(g);
    
    remove("reservations.txt");
    rename("reservations2.txt", "reservations.txt");
}
