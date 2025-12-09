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
void on_Deconnexion_clicked (GtkButton *button, gpointer user_data);

// Callback pour DashEntraineur
void on_login_equipement_clicked (GtkButton *button, gpointer user_data);
void on_retour_dashboard_clicked (GtkButton *button, gpointer user_data);

// Callback pour validation équipements
void on_valider_clicked (GtkButton *button, gpointer user_data);

// Callback pour fermer
void on_window_destroy (GtkWidget *widget, gpointer user_data);


void
on_connecter_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_equipementAdmin_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_Deconnexion_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_retour_dashboard_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRechercher_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_supprimer_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_modifier_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_ajouter_clicked                     (GtkButton       *button,
                                        gpointer         user_data);
#endif

void
on_retour_dashboard_clicked            (GtkButton       *button,
                                        gpointer         user_data);
