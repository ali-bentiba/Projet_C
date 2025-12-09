#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "fonction.h"

// Utiliser le builder depuis support.c
extern GtkBuilder *builder;

// ============================================
// Interface de CHOIX (Login ou Signup)
// ============================================

void on_to_login_clicked (GtkButton *button, gpointer user_data)
{
    // AJOUTER ICI :
    if (!builder) {
        g_print("Erreur: builder est NULL dans on_to_login_clicked\n");
        return;
    }
    
    // Fermer fenêtre de choix
    GtkWidget *window_choix = GTK_WIDGET(gtk_builder_get_object(builder, "window_choix"));
    if (window_choix) {
        gtk_widget_destroy(window_choix);
    }
    
    // Ouvrir fenêtre login
    GtkWidget *window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
    if (window_login) {
        gtk_widget_show_all(window_login);
    }
}

void on_to_signup_clicked (GtkButton *button, gpointer user_data)
{
    // AJOUTER ICI :
    if (!builder) {
        g_print("Erreur: builder est NULL dans on_to_signup_clicked\n");
        return;
    }
    
    // Fermer fenêtre de choix
    GtkWidget *window_choix = GTK_WIDGET(gtk_builder_get_object(builder, "window_choix"));
    if (window_choix) {
        gtk_widget_destroy(window_choix);
    }
    
    // Ouvrir fenêtre signup
    GtkWidget *window_signup = GTK_WIDGET(gtk_builder_get_object(builder, "window_signup"));
    if (window_signup) {
        gtk_widget_show_all(window_signup);
    }
}

// ============================================
// Interface SIGNUP (Inscription)
// ============================================

void on_signup_clicked (GtkButton *button, gpointer user_data)
{
    // AJOUTER ICI :
    if (!builder) {
        g_print("Erreur: builder est NULL dans on_signup_clicked\n");
        return;
    }
    
    // Récupérer les widgets depuis le builder
    GtkEntry *entry_fullname = GTK_ENTRY(gtk_builder_get_object(builder, "fullname"));
    GtkEntry *entry_username = GTK_ENTRY(gtk_builder_get_object(builder, "username"));
    GtkEntry *entry_password = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
    GtkSpinButton *spin_role = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "role"));
    
    // AJOUTER vérification des widgets :
    if (!entry_fullname || !entry_username || !entry_password || !spin_role) {
        g_print("Erreur: Un ou plusieurs widgets non trouvés dans signup\n");
        return;
    }
    
    // Récupérer les valeurs
    const char *fullname = gtk_entry_get_text(entry_fullname);
    const char *username = gtk_entry_get_text(entry_username);
    const char *password = gtk_entry_get_text(entry_password);
    int role = (int)gtk_spin_button_get_value(spin_role);
    
    // Vérifier si tous les champs sont remplis
    if (strlen(fullname) == 0 || strlen(username) == 0 || strlen(password) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_WARNING,
                                                  GTK_BUTTONS_OK,
                                                  "Veuillez remplir tous les champs !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Ajouter à fichier
    ajouter((char*)fullname, (char*)username, (char*)password, role);
    
    // Fermer fenêtre signup
    GtkWidget *window_signup = GTK_WIDGET(gtk_builder_get_object(builder, "window_signup"));
    if (window_signup) {
        gtk_widget_destroy(window_signup);
    }
    
    // Ouvrir directement login après inscription
    GtkWidget *window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
    if (window_login) {
        gtk_widget_show_all(window_login);
    }
}

// ============================================
// Interface LOGIN (Authentification)
// ============================================

void on_bouttonlogin_clicked (GtkButton *button, gpointer user_data)
{
    // AJOUTER ICI :
    if (!builder) {
        g_print("Erreur: builder est NULL dans on_bouttonlogin_clicked\n");
        return;
    }
    
    // Récupérer les widgets
    GtkEntry *entry_username = GTK_ENTRY(gtk_builder_get_object(builder, "Username"));
    GtkEntry *entry_password = GTK_ENTRY(gtk_builder_get_object(builder, "Password"));
    
    // AJOUTER vérification des widgets :
    if (!entry_username || !entry_password) {
        g_print("Erreur: Widgets login non trouvés\n");
        // Essayer d'autres noms
        entry_username = GTK_ENTRY(gtk_builder_get_object(builder, "username"));
        entry_password = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
        
        if (!entry_username || !entry_password) {
            g_print("Erreur: Widgets login (variantes) non trouvés non plus\n");
            return;
        }
    }
    
    // Récupérer les valeurs
    const char *username = gtk_entry_get_text(entry_username);
    const char *password = gtk_entry_get_text(entry_password);
    
    // Vérifier si les champs sont vides
    if (strlen(username) == 0 || strlen(password) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_WARNING,
                                                  GTK_BUTTONS_OK,
                                                  "Veuillez entrer username et password !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    int role;
    if (verifier((char*)username, (char*)password, &role)) {
        // Authentification réussie
        // Fermer fenêtre login
        GtkWidget *window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
        if (window_login) {
            gtk_widget_destroy(window_login);
        }
        
        // Ouvrir fenêtre selon rôle
        GtkWidget *window_role = NULL;
        switch(role) {
            case 1: // Admin
                window_role = GTK_WIDGET(gtk_builder_get_object(builder, "DashAdmin"));
                break;
            case 2: // Entraineur
                window_role = GTK_WIDGET(gtk_builder_get_object(builder, "DashEntraineur"));
                break;
            case 3: // Membre
                window_role = GTK_WIDGET(gtk_builder_get_object(builder, "DashMembre"));
                break;
            default:
                window_role = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
        }
        if (window_role) {
            gtk_widget_show_all(window_role);
        }
    } else {
        // Échec authentification
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Username ou mot de passe incorrect !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// ============================================
// DASHADMIN - Gestion des équipements
// ============================================

void on_equipementAdmin_clicked (GtkButton *button, gpointer user_data)
{
    if (!builder) {
        g_print("Erreur: builder est NULL dans on_equipementAdmin_clicked\n");
        return;
    }
    // Fermer DashAdmin
    GtkWidget *window_dashadmin = GTK_WIDGET(gtk_builder_get_object(builder, "DashAdmin"));
    if (window_dashadmin) {
        gtk_widget_destroy(window_dashadmin);
    }
    // Ouvrir interface Gestion des équipements
    GtkWidget *window_equipements = GTK_WIDGET(gtk_builder_get_object(builder, "Gestion des equippements/Admin"));
    if (window_equipements) {
        gtk_widget_show_all(window_equipements);
    } else {
        g_print("Erreur: Gestion des equippements/Admin non trouvée\n");
    }
}

// ============================================
// DASHADMIN - Déconnexion
// ============================================

void on_Deconnexion_clicked (GtkButton *button, gpointer user_data)
{
 if (!builder) {
        g_print("Erreur: builder est NULL dans on_Deconnexion_clicked\n");
        return;
    }
    
    // Fermer DashAdmin
    GtkWidget *window_dashadmin = GTK_WIDGET(gtk_builder_get_object(builder, "DashAdmin"));
    if (window_dashadmin) {
        gtk_widget_destroy(window_dashadmin);
    }
    
    // Ouvrir window_login
    GtkWidget *window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
    if (window_login) {
        gtk_widget_show_all(window_login);
    } else {
        g_print("Erreur: window_login non trouvée après déconnexion\n");
    }

}

// ============================================
// DASHENTRAINEUR - Équipements
// ============================================

void on_login_equipement_clicked (GtkButton *button, gpointer user_data)
{
if (!builder) {
        g_print("Erreur: builder est NULL dans on_login_equipement_clicked\n");
        return;
    }
    
    // Fermer DashEntraineur
    GtkWidget *window_dashentraineur = GTK_WIDGET(gtk_builder_get_object(builder, "DashEntraineur"));
    if (window_dashentraineur) {
        gtk_widget_destroy(window_dashentraineur);
    }
    
    // Ouvrir interface Équipements entraineurs
    GtkWidget *window_equipements = GTK_WIDGET(gtk_builder_get_object(builder, "Equippements_entraineurs"));
    if (window_equipements) {
        gtk_widget_show_all(window_equipements);
    } else {
        g_print("Erreur: Equippements_entraineurs non trouvée\n");
        
        // Débogage : vérifier le nom exact
        GList *objects = gtk_builder_get_objects(builder);
        GtkWidget *found = NULL;
        while (objects) {
            if (GTK_IS_WINDOW(objects->data)) {
                const char *type = G_OBJECT_TYPE_NAME(objects->data);
                g_print("Fenêtre disponible: %s\n", type);
                // Vérifier si c'est la fenêtre qu'on cherche
                if (strstr(type, "Equippements") != NULL) {
                    found = GTK_WIDGET(objects->data);
                    g_print("Fenêtre Equippements trouvée par type\n");
                }
            }
            objects = objects->next;
        }
        
        if (found) {
            gtk_widget_show_all(found);
        }
    }
}

// ============================================
// DASHENTRAINEUR - Retour au login
// ============================================

void on_retour_dashboard_clicked (GtkButton *button, gpointer user_data)
{
if (!builder) {
        g_print("Erreur: builder est NULL dans on_retour_dashboard_clicked\n");
        return;
    }
    
    // Fermer DashEntraineur
    GtkWidget *window_dashentraineur = GTK_WIDGET(gtk_builder_get_object(builder, "DashEntraineur"));
    if (window_dashentraineur) {
        gtk_widget_destroy(window_dashentraineur);
    }
    
    // Ouvrir window_login
    GtkWidget *window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
    if (window_login) {
        gtk_widget_show_all(window_login);
    } else {
        g_print("Erreur: window_login non trouvée\n");
    }
}

// ============================================
// EQUIPEMENTS ENTRAINEURS - Validation
// ============================================

void on_valider_clicked (GtkButton *button, gpointer user_data)
{
    // AJOUTER ICI :
    if (!builder) {
        g_print("Erreur: builder est NULL dans on_valider_clicked\n");
        return;
    }
    
    // Récupérer les widgets
    GtkComboBoxText *combo_entraineur = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "entraineur"));
    GtkComboBoxText *combo_type = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "type"));
    GtkComboBoxText *combo_nomEquipement = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "nomEquipement"));
    GtkSpinButton *spin_quantite = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "quantite"));
    GtkComboBoxText *combo_salle = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "salle"));
    GtkSpinButton *spin_jour = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "Jour"));
    GtkSpinButton *spin_mois = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "Mois"));
    GtkSpinButton *spin_annee = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "Annee"));
    GtkComboBoxText *combo_periode = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "periode"));
    GtkLabel *label_sortie = GTK_LABEL(gtk_builder_get_object(builder, "sortieValider"));
    
    // AJOUTER vérification des widgets :
    if (!combo_entraineur || !combo_type || !combo_nomEquipement || !spin_quantite ||
        !combo_salle || !spin_jour || !spin_mois || !spin_annee || !combo_periode || !label_sortie) {
        g_print("Erreur: Un ou plusieurs widgets non trouvés dans Equippements_entraineurs\n");
        return;
    }
// ============================================
// Fermeture d'application
// ============================================

void on_window_destroy(GtkWidget *widget, gpointer user_data)
{
    // Fermer l'application quand la fenêtre principale est fermée
    gtk_main_quit();
}







int statut_equipement = 1;
void on_radio_libre_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
        statut_equipement = 1;
}
void on_radio_reserve_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
        statut_equipement = 2;
}
void on_radio_maintenance_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
        statut_equipement = 3;
}
void on_rechercher_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *entry_recherche = lookup_widget(button, "entry_recherche");
    if(!entry_recherche) return;
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
    if(strlen(id_text) == 0) return;
    int id = atoi(id_text);
    printf("Recherche équipement ID: %d\n", id);
    if(id == 1001) {
        GtkWidget *entry_nom = lookup_widget(button, "entry_nom");
        GtkWidget *entry_type = lookup_widget(button, "entry_type");
        GtkWidget *entry_salle = lookup_widget(button, "entry_salle");
        GtkWidget *entry_date = lookup_widget(button, "entry_date");
        if(entry_nom) gtk_entry_set_text(GTK_ENTRY(entry_nom), "Tapis roulant");
        if(entry_type) gtk_entry_set_text(GTK_ENTRY(entry_type), "Cardio");
        if(entry_salle) gtk_entry_set_text(GTK_ENTRY(entry_salle), "Salle 1");
        if(entry_date) gtk_entry_set_text(GTK_ENTRY(entry_date), "01/01/2023");
        GtkWidget *radio_libre = lookup_widget(button, "radio_libre");
        if(radio_libre) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_libre), TRUE);
            statut_equipement = 1;
        }
    }
}
void on_ajouter_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *entry_nom = lookup_widget(button, "entry_nom");
    GtkWidget *entry_type = lookup_widget(button, "entry_type");
    GtkWidget *entry_salle = lookup_widget(button, "entry_salle");
    GtkWidget *entry_date = lookup_widget(button, "entry_date");
    if(!entry_nom || !entry_type || !entry_salle || !entry_date) return;
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(entry_type));
    const gchar *salle = gtk_entry_get_text(GTK_ENTRY(entry_salle));
    const gchar *date = gtk_entry_get_text(GTK_ENTRY(entry_date));
    char msg_statut[50] = "";
    get_statut_equipement(statut_equipement, msg_statut);
    printf("Ajout équipement:\n");
    printf("  Nom: %s\n", nom);
    printf("  Type: %s\n", type);
    printf("  Salle: %s\n", salle);
    printf("  Date: %s\n", date);
    printf("  Statut: %s (code: %d)\n", msg_statut, statut_equipement);
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(gtk_widget_get_toplevel(button)),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Équipement ajouté avec succès!\nStatut: %s",
        msg_statut);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
void on_modifier_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *entry_recherche = lookup_widget(button, "entry_recherche");
    if(!entry_recherche) return;
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_recherche));
    if(strlen(id_text) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(button)),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Veuillez d'abord rechercher un équipement");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    int id = atoi(id_text);
    char msg_statut[50] = "";
    get_statut_equipement(statut_equipement, msg_statut);
    printf("Modification équipement ID %d\n", id);
    printf("Nouveau statut: %s (code: %d)\n", msg_statut, statut_equipement);
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(gtk_widget_get_toplevel(button)),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Statut modifié pour l'équipement ID %d\nNouveau statut: %s",
        id, msg_statut);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
void on_supprimer_clicked(GtkWidget *button, gpointer user_data) {
}



void
on_connecter_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_equipementAdmin_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}

void
on_buttonRechercher_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_supprimer_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_modifier_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_ajouter_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}

void
on_retour_dashboard_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}

