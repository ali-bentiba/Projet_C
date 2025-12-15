#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <gtk/gtk.h>

// Callback pour choix Login/Signup
void on_to_login_clicked (GtkButton *button, gpointer user_data);
void on_to_signup_clicked (GtkButton *button, gpointer user_data);

// Callback pour le bouton "Sign up" (inscription)
void on_signup_clicked (GtkButton *button, gpointer user_data);

// Callback pour le bouton "Login" (authentification)
void on_bouttonlogin_clicked (GtkButton *button, gpointer user_data);

// Callback pour DashAdmin
void on_equipementAdmin_clicked (GtkButton *button, gpointer user_data);
void on_toadmin_clicked (GtkButton *button, gpointer user_data);
void on_Deconnexion_clicked (GtkButton *button, gpointer user_data);

// Callback pour DashEntraineur
void on_login_equipement_clicked (GtkButton *button, gpointer user_data);
void on_retour_dashboard_clicked (GtkButton *button, gpointer user_data);
void on_seance_clicked (GtkButton *button, gpointer user_data);

// Callback pour validation équipements
void on_valider_clicked (GtkButton *button, gpointer user_data);

// Callback pour chargement des donnees d'apres l'admin
void on_treeview_reservations_row_activated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);

// Callback pour le bouton SUPPRIMER 
void on_supprimer_clicked (GtkButton *button, gpointer user_data);

// Callback pour le bouton MODIFIER
void on_modifier_clicked (GtkButton *button, gpointer user_data);

// Callback pour aller à l'interface Entraineur
void on_a_interfaceEntraineur_clicked (GtkButton *button, gpointer user_data);


#endif
