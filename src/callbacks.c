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

// Variables globales - déclarez-les ici
GtkWidget *window_choix = NULL;
GtkWidget *window_signup = NULL;
GtkWidget *window_login = NULL;

// ============================================
// Interface de CHOIX (Login ou Signup)
// ============================================
void on_to_login_clicked (GtkButton *button, gpointer user_data)
{   
    // Fermer TOUTES les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Créer nouvelle fenêtre
    GtkWidget *login = create_window_login();
    gtk_widget_show_all(login);
}

void on_to_signup_clicked (GtkButton *button, gpointer user_data)
{

    // Fermer TOUTES les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Créer nouvelle fenêtre
    GtkWidget *signup = create_window_signup();
    gtk_widget_show_all(signup);
}

// ============================================
// Interface SIGNUP (Inscription)
// ============================================

void on_signup_clicked (GtkButton *button, gpointer user_data)
{
    // Utilisez GTK_WIDGET(button)
    GtkWidget *window_signup = lookup_widget(GTK_WIDGET(button), "window_signup");
    GtkEntry *entry_fullname = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "fullname"));
    GtkEntry *entry_username = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "username"));
    GtkEntry *entry_password = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "password"));
    GtkSpinButton *spin_role = GTK_SPIN_BUTTON(lookup_widget(GTK_WIDGET(button), "role"));
    
    // Vérification
    if (!entry_fullname || !entry_username || !entry_password || !spin_role) {
        g_print("Erreur: Widgets non trouvés\n");
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
    
    // Fermer toutes les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Ouvrir login
    GtkWidget *login = create_window_login();
    gtk_widget_show_all(login);
}

// ============================================
// Interface LOGIN (Authentification)
// ============================================

void on_bouttonlogin_clicked (GtkButton *button, gpointer user_data)
{
    g_print("\n=== DÉBUT LOGIN ===\n");
    
    GtkEntry *entry_username = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "Username"));
    GtkEntry *entry_password = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "Password"));
    
    if (!entry_username || !entry_password) {
        entry_username = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "username"));
        entry_password = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "password"));
    }
    
    if (!entry_username || !entry_password) {
        g_print("ERREUR: Widgets login non trouvés\n");
        return;
    }
    
    const char *username = gtk_entry_get_text(entry_username);
    const char *password = gtk_entry_get_text(entry_password);
    
    g_print("DEBUG LOGIN: username='%s' (longueur=%zu)\n", username, strlen(username));
    g_print("DEBUG LOGIN: password='%s' (longueur=%zu)\n", password, strlen(password));
    
    if (strlen(username) == 0 || strlen(password) == 0) {
        g_print("DEBUG: Champs vides\n");
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
    g_print("DEBUG: Appel verifier()...\n");
    int result = verifier((char*)username, (char*)password, &role);
    g_print("DEBUG: verifier() a retourné %d, role=%d\n", result, role);
    
    if (result) {
        g_print("DEBUG: Authentification réussie! Role=%d\n", role);
        
        GList *list = gtk_window_list_toplevels();
        for (GList *l = list; l != NULL; l = l->next) {
            gtk_widget_hide(GTK_WIDGET(l->data));
        }
        g_list_free(list);
        
        GtkWidget *window_role = NULL;
        switch(role) {
            case 1: // Admin
                g_print("DEBUG: Ouverture DashAdmin\n");
                window_role = create_DashAdmin();
                break;
            case 2: // Entraineur
                g_print("DEBUG: Ouverture DashEntraineur\n");
                window_role = create_DashEntraineur();
                break;
            case 3: // Membre
                g_print("DEBUG: Ouverture DashMembre\n");
                window_role = create_DAshMembre();
                break;
            default:
                g_print("DEBUG: Role inconnu, retour login\n");
                window_role = create_window_login();
        }
        
        if (window_role) {
            gtk_widget_show_all(window_role);
        }
    } else {
        g_print("DEBUG: Échec authentification\n");
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Username ou mot de passe incorrect !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    
    g_print("=== FIN LOGIN ===\n\n");
}

// ============================================
// DASHADMIN - Gestion des équipements
// ============================================

void on_equipementAdmin_clicked (GtkButton *button, gpointer user_data)
{
    // Fermer toutes les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Ouvrir interface Gestion des équipements
    GtkWidget *window_equipements = create_Gestion_des_equippements_Admin();
    if (window_equipements) {
        // Récupérer le treeview dans la nouvelle fenêtre
        GtkTreeView *treeview = GTK_TREE_VIEW(lookup_widget(window_equipements, "treeview_reservations"));
        if (treeview) {
            afficher_reservations_dans_treeview(treeview);
        }
        
        gtk_widget_show_all(window_equipements);
    }
}

// ============================================
// DASHADMIN - Déconnexion
// ============================================

void on_Deconnexion_clicked (GtkButton *button, gpointer user_data)
{
    // Fermer toutes les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Ouvrir window_login
    GtkWidget *window_login = create_window_login();
    gtk_widget_show_all(window_login);
}

// ============================================
// From DashAdmin to Entraineur Admin (Dhia)
// ============================================

void on_toadmin_clicked (GtkButton *button, gpointer user_data)
{
    // Fermer toutes les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Ouvrir la fenêtre Gestion entraineurs/Admin
    GtkWidget *window_gestion_admin = create_Gestion_entraineurs_Admin();
    gtk_widget_show_all(window_gestion_admin);
}

// ============================================
// DASHENTRAINEUR - Équipements
// ============================================

void on_login_equipement_clicked (GtkButton *button, gpointer user_data)
{
    // Fermer toutes les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Ouvrir interface Équipements entraineurs
    GtkWidget *window_equipements = create_Equippements_entraineurs();
    gtk_widget_show_all(window_equipements);
}

// ============================================
// DASHENTRAINEUR - Retour au login
// ============================================

void on_retour_dashboard_clicked (GtkButton *button, gpointer user_data)
{
    // Fermer toutes les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Ouvrir window_login
    GtkWidget *window_login = create_window_login();
    gtk_widget_show_all(window_login);
}

// ======================================================
// From DashEntraineur to Eantraineur/Entraineur (Dhia)
// ======================================================

void on_seance_clicked (GtkButton *button, gpointer user_data)
{
    // Fermer toutes les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Ouvrir la fenêtre Espace entraineur/entraineur
    GtkWidget *window_espace_entraineur = create_Espace_entraineur_entraineur();
    gtk_widget_show_all(window_espace_entraineur);
}

// ============================================
// EQUIPEMENTS ENTRAINEURS - Validation
// ============================================

void on_valider_clicked (GtkButton *button, gpointer user_data)
{
    // Récupérer les widgets depuis la fenêtre parente
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    // CHANGEMENT ICI : Utiliser GtkComboBox au lieu de GtkComboBoxText
    GtkComboBox *combo_entraineur = GTK_COMBO_BOX(lookup_widget(window, "entraineur"));
    GtkComboBox *combo_type = GTK_COMBO_BOX(lookup_widget(window, "type"));
    GtkComboBox *combo_nomEquipement = GTK_COMBO_BOX(lookup_widget(window, "nomEquipement"));
    GtkSpinButton *spin_quantite = GTK_SPIN_BUTTON(lookup_widget(window, "quantite"));
    GtkComboBox *combo_salle = GTK_COMBO_BOX(lookup_widget(window, "salle"));
    GtkSpinButton *spin_jour = GTK_SPIN_BUTTON(lookup_widget(window, "Jour"));
    GtkSpinButton *spin_mois = GTK_SPIN_BUTTON(lookup_widget(window, "Mois"));
    GtkSpinButton *spin_annee = GTK_SPIN_BUTTON(lookup_widget(window, "Annee"));
    GtkComboBox *combo_periode = GTK_COMBO_BOX(lookup_widget(window, "periode"));
    GtkLabel *label_sortie = GTK_LABEL(lookup_widget(window, "sortieValider"));
    
    // Vérification des widgets
    if (!combo_entraineur || !combo_type || !combo_nomEquipement || !spin_quantite ||
        !combo_salle || !spin_jour || !spin_mois || !spin_annee || !combo_periode || !label_sortie) {
        g_print("Erreur: Un ou plusieurs widgets non trouvés\n");
        return;
    }
    
    // CHANGEMENT ICI : Récupérer le texte différemment
    gchar *entraineur = gtk_combo_box_get_active_text(combo_entraineur);
    gchar *type = gtk_combo_box_get_active_text(combo_type);
    gchar *equipement = gtk_combo_box_get_active_text(combo_nomEquipement);
    gchar *salle = gtk_combo_box_get_active_text(combo_salle);
    gchar *periode = gtk_combo_box_get_active_text(combo_periode);
    
    int quantite = (int)gtk_spin_button_get_value(spin_quantite);
    int jour = (int)gtk_spin_button_get_value(spin_jour);
    int mois = (int)gtk_spin_button_get_value(spin_mois);
    int annee = (int)gtk_spin_button_get_value(spin_annee);
    
    // Vérifier si les champs sont vides
    if (!entraineur || !type || !equipement || !salle || !periode ||
        strlen(entraineur) == 0 || strlen(type) == 0 || strlen(equipement) == 0 ||
        strlen(salle) == 0 || strlen(periode) == 0) {
        gtk_label_set_text(label_sortie, "remplir tous les champs");
        
        // Libérer la mémoire
        g_free(entraineur); g_free(type); g_free(equipement);
        g_free(salle); g_free(periode);
        return;
    }
    
    // Créer et remplir la structure
    ReservationEquipement reservation;
    strcpy(reservation.entraineur, entraineur);
    strcpy(reservation.type_equipement, type);
    strcpy(reservation.nom_equipement, equipement);
    reservation.quantite = quantite;
    strcpy(reservation.salle, salle);
    reservation.jour = jour;
    reservation.mois = mois;
    reservation.annee = annee;
    strcpy(reservation.periode, periode);
    
    // Enregistrer dans le fichier
    ajouter_reservation(&reservation);
    
    // Message simple de succès
    gtk_label_set_text(label_sortie, "Validation avec succes");
    
    // CHANGEMENT ICI : Libérer la mémoire
    g_free(entraineur);
    g_free(type);
    g_free(equipement);
    g_free(salle);
    g_free(periode);
}

// ============================================
// GESTION EQUIPEMENTS - TreeView/Admin
// ============================================

void on_treeview_reservations_row_activated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
    // Déclaration des variables
    GtkTreeIter iter;
    gchar *entraineur = NULL, *type = NULL, *equipement = NULL;
    gchar *quantite_str = NULL, *salle = NULL, *date = NULL, *periode = NULL;
    
    // Vérifier que treeview et path ne sont pas NULL
    if (!treeview || !path) {
        g_print("Erreur: TreeView ou chemin invalide\n");
        return;
    }
    
    // Récupérer le modèle
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    if (!model) {
        g_print("Erreur: Modèle TreeView non trouvé\n");
        return;
    }
    
    // Obtenir l'itérateur pour la ligne sélectionnée
    if (gtk_tree_model_get_iter(model, &iter, path)) 
    {
        // Récupérer les valeurs de la ligne sélectionnée
        gtk_tree_model_get(model, &iter, 
                          0, &entraineur, 
                          1, &type, 
                          2, &equipement, 
                          3, &quantite_str, 
                          4, &salle, 
                          5, &date, 
                          6, &periode, 
                          -1);
        
        // Vérifier que toutes les données ont été récupérées
        if (!entraineur || !type || !equipement || !quantite_str || 
            !salle || !date || !periode) {
            g_print("Avertissement: Certaines données sont manquantes\n");
        }
        
        // Création de la structure ReservationEquipement
        ReservationEquipement reservation;
        
        // Initialiser la structure avec des valeurs par défaut
        memset(&reservation, 0, sizeof(ReservationEquipement));
        reservation.quantite = 0;
        reservation.jour = 1;
        reservation.mois = 1;
        reservation.annee = 2024;
        
        // Copier les données strings avec vérification et sécurité
        if (entraineur) {
            strncpy(reservation.entraineur, entraineur, sizeof(reservation.entraineur) - 1);
            reservation.entraineur[sizeof(reservation.entraineur) - 1] = '\0';
        } else {
            reservation.entraineur[0] = '\0';
        }
        
        if (type) {
            strncpy(reservation.type_equipement, type, sizeof(reservation.type_equipement) - 1);
            reservation.type_equipement[sizeof(reservation.type_equipement) - 1] = '\0';
        } else {
            reservation.type_equipement[0] = '\0';
        }
        
        if (equipement) {
            strncpy(reservation.nom_equipement, equipement, sizeof(reservation.nom_equipement) - 1);
            reservation.nom_equipement[sizeof(reservation.nom_equipement) - 1] = '\0';
        } else {
            reservation.nom_equipement[0] = '\0';
        }
        
        if (salle) {
            strncpy(reservation.salle, salle, sizeof(reservation.salle) - 1);
            reservation.salle[sizeof(reservation.salle) - 1] = '\0';
        } else {
            reservation.salle[0] = '\0';
        }
        
        if (periode) {
            strncpy(reservation.periode, periode, sizeof(reservation.periode) - 1);
            reservation.periode[sizeof(reservation.periode) - 1] = '\0';
        } else {
            reservation.periode[0] = '\0';
        }
        
        // Convertir quantité de string à int avec vérification
        if (quantite_str) {
            char *endptr;
            reservation.quantite = (int)strtol(quantite_str, &endptr, 10);
            if (*endptr != '\0' || reservation.quantite < 0) {
                g_print("Avertissement: Quantité invalide: %s, utilisation de 0\n", quantite_str);
                reservation.quantite = 0;
            }
        }
        
        // Parser la date "15/01/2024" -> jour, mois, annee
        if (date) {
            int jour = 0, mois = 0, annee = 0;
            if (sscanf(date, "%d/%d/%d", &jour, &mois, &annee) == 3) {
                // Validation des valeurs de date
                if (jour >= 1 && jour <= 31) {
                    reservation.jour = jour;
                } else {
                    g_print("Avertissement: Jour invalide: %d\n", jour);
                    reservation.jour = 1;
                }
                
                if (mois >= 1 && mois <= 12) {
                    reservation.mois = mois;
                } else {
                    g_print("Avertissement: Mois invalide: %d\n", mois);
                    reservation.mois = 1;
                }
                
                if (annee >= 2000 && annee <= 2100) {
                    reservation.annee = annee;
                } else {
                    g_print("Avertissement: Année invalide: %d\n", annee);
                    reservation.annee = 2024;
                }
            } else {
                g_print("Avertissement: Format de date invalide: %s\n", date);
                reservation.jour = 1;
                reservation.mois = 1;
                reservation.annee = 2024;
            }
        }
        
        // ICI TU PEUX UTILISER LA STRUCTURE 'reservation' :
        // - Pour afficher les détails dans une boîte de dialogue
        // - Pour remplir un formulaire de modification
        // - Pour supprimer la réservation
        // - Pour exporter les données
        
        // Exemple: Afficher les informations dans la console
        g_print("\n=== Réservation sélectionnée ===\n");
        g_print("Entraîneur: %s\n", reservation.entraineur);
        g_print("Type: %s\n", reservation.type_equipement);
        g_print("Équipement: %s\n", reservation.nom_equipement);
        g_print("Quantité: %d\n", reservation.quantite);
        g_print("Salle: %s\n", reservation.salle);
        g_print("Date: %02d/%02d/%04d\n", reservation.jour, reservation.mois, reservation.annee);
        g_print("Période: %s\n", reservation.periode);
        g_print("===============================\n");
        
        // Exemple: Remplir un formulaire de modification
        // (À implémenter selon ton interface)
        // fill_modification_form(&reservation);
        
        // Libérer la mémoire allouée par gtk_tree_model_get
        if (entraineur) g_free(entraineur);
        if (type) g_free(type);
        if (equipement) g_free(equipement);
        if (quantite_str) g_free(quantite_str);
        if (salle) g_free(salle);
        if (date) g_free(date);
        if (periode) g_free(periode);
    }
    else
    {
        g_print("Erreur: Impossible de récupérer la ligne sélectionnée\n");
    }
}

// ============================================
// 		SUPPRIMER RÉSERVATION
// ============================================

void on_supprimer_clicked (GtkButton *button, gpointer user_data)
{
    // Récupérer la fenêtre parente
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    if (!window || !GTK_IS_WINDOW(window)) {
        g_print("Erreur: Fenêtre parente non trouvée\n");
        return;
    }
    
    // Récupérer le TreeView via lookup_widget
    GtkTreeView *treeview = GTK_TREE_VIEW(lookup_widget(window, "treeview_reservations"));
    if (!treeview) {
        g_print("Erreur: treeview_reservations non trouvé\n");
        
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Erreur: Liste des réservations non trouvée !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Récupérer la sélection
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) 
    {
        // Récupérer les valeurs
        gchar *entraineur = NULL, *type = NULL, *equipement = NULL;
        gchar *quantite_str = NULL, *salle = NULL, *date = NULL, *periode = NULL;
        
        gtk_tree_model_get(model, &iter, 
                          0, &entraineur, 
                          1, &type, 
                          2, &equipement, 
                          3, &quantite_str, 
                          4, &salle, 
                          5, &date, 
                          6, &periode, -1);
        
        // Vérifier que toutes les données sont présentes
        if (!entraineur || !type || !equipement || !quantite_str || !salle || !date || !periode) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_ERROR,
                                                      GTK_BUTTONS_OK,
                                                      "Données incomplètes !");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            
            // Libérer ce qui a été alloué
            if (entraineur) g_free(entraineur);
            if (type) g_free(type);
            if (equipement) g_free(equipement);
            if (quantite_str) g_free(quantite_str);
            if (salle) g_free(salle);
            if (date) g_free(date);
            if (periode) g_free(periode);
            
            return;
        }
        
        // Créer la structure ReservationEquipement
        ReservationEquipement reservation;
        
        // Copier les données
        strncpy(reservation.entraineur, entraineur, sizeof(reservation.entraineur) - 1);
        reservation.entraineur[sizeof(reservation.entraineur) - 1] = '\0';
        
        strncpy(reservation.type_equipement, type, sizeof(reservation.type_equipement) - 1);
        reservation.type_equipement[sizeof(reservation.type_equipement) - 1] = '\0';
        
        strncpy(reservation.nom_equipement, equipement, sizeof(reservation.nom_equipement) - 1);
        reservation.nom_equipement[sizeof(reservation.nom_equipement) - 1] = '\0';
        
        // Convertir la quantité
        reservation.quantite = atoi(quantite_str);
        
        strncpy(reservation.salle, salle, sizeof(reservation.salle) - 1);
        reservation.salle[sizeof(reservation.salle) - 1] = '\0';
        
        // Parser la date "15/01/2024" -> jour, mois, annee
        if (date) {
            if (sscanf(date, "%d/%d/%d", &reservation.jour, &reservation.mois, &reservation.annee) != 3) {
                reservation.jour = 1;
                reservation.mois = 1;
                reservation.annee = 2024;
            }
        }
        
        strncpy(reservation.periode, periode, sizeof(reservation.periode) - 1);
        reservation.periode[sizeof(reservation.periode) - 1] = '\0';
        
        // Demander confirmation
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_QUESTION,
                                                  GTK_BUTTONS_YES_NO,
                                                  "Voulez-vous vraiment supprimer cette réservation ?\n\n"
                                                  "Entraîneur: %s\n"
                                                  "Équipement: %s\n"
                                                  "Date: %s\n"
                                                  "Période: %s",
                                                  entraineur, equipement, date, periode);
        
        gint result = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (result == GTK_RESPONSE_YES) {
            // Appeler la fonction de suppression
            supprimer_reservation(&reservation);
            
            // Rafraîchir l'affichage
            afficher_reservations_dans_treeview(treeview);
            
            // Message de confirmation
            GtkWidget *info_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                           GTK_DIALOG_MODAL,
                                                           GTK_MESSAGE_INFO,
                                                           GTK_BUTTONS_OK,
                                                           "Réservation supprimée avec succès !");
            gtk_dialog_run(GTK_DIALOG(info_dialog));
            gtk_widget_destroy(info_dialog);
        }
        else {
            // Message d'annulation
            GtkWidget *info_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                           GTK_DIALOG_MODAL,
                                                           GTK_MESSAGE_INFO,
                                                           GTK_BUTTONS_OK,
                                                           "Suppression annulée.");
            gtk_dialog_run(GTK_DIALOG(info_dialog));
            gtk_widget_destroy(info_dialog);
        }
        
        // Libérer la mémoire
        g_free(entraineur);
        g_free(type);
        g_free(equipement);
        g_free(quantite_str);
        g_free(salle);
        g_free(date);
        g_free(periode);
    }
    else {
        // Aucune sélection
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_WARNING,
                                                  GTK_BUTTONS_OK,
                                                  "Veuillez sélectionner une réservation à supprimer !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// ============================================
// MODIFIER RÉSERVATION (Bouton)
// ============================================

void on_modifier_clicked (GtkButton *button, gpointer user_data)
{
    // Récupérer la fenêtre parente
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    if (!window || !GTK_IS_WINDOW(window)) {
        g_print("Erreur: Fenêtre parente non trouvée\n");
        return;
    }
    
    // Récupérer le TreeView via lookup_widget
    GtkTreeView *treeview = GTK_TREE_VIEW(lookup_widget(window, "treeview_reservations"));
    if (!treeview) {
        g_print("Erreur: treeview_reservations non trouvé\n");
        
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Erreur: Liste des réservations non trouvée !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Récupérer le Label pour les messages
    GtkLabel *label_sortie = GTK_LABEL(lookup_widget(window, "sortieRechercher"));
    
    // Récupérer la sélection
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) 
    {
        // Récupérer les valeurs de la ligne sélectionnée (ANCIENNES)
        gchar *entraineur = NULL, *type = NULL, *equipement = NULL;
        gchar *quantite_str = NULL, *salle = NULL, *date = NULL, *periode = NULL;
        
        gtk_tree_model_get(model, &iter, 
                          0, &entraineur, 
                          1, &type, 
                          2, &equipement, 
                          3, &quantite_str, 
                          4, &salle, 
                          5, &date, 
                          6, &periode, -1);
        
        // Vérifier que toutes les données sont présentes
        if (!entraineur || !type || !equipement || !quantite_str || !salle || !date || !periode) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_ERROR,
                                                      GTK_BUTTONS_OK,
                                                      "Données incomplètes !");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            
            // Libérer la mémoire
            if (entraineur) g_free(entraineur);
            if (type) g_free(type);
            if (equipement) g_free(equipement);
            if (quantite_str) g_free(quantite_str);
            if (salle) g_free(salle);
            if (date) g_free(date);
            if (periode) g_free(periode);
            
            return;
        }
        
        // Créer la structure ANCIENNE ReservationEquipement
        ReservationEquipement ancienne;
        
        // Copier les anciennes données avec sécurité
        strncpy(ancienne.entraineur, entraineur, sizeof(ancienne.entraineur) - 1);
        ancienne.entraineur[sizeof(ancienne.entraineur) - 1] = '\0';
        
        strncpy(ancienne.type_equipement, type, sizeof(ancienne.type_equipement) - 1);
        ancienne.type_equipement[sizeof(ancienne.type_equipement) - 1] = '\0';
        
        strncpy(ancienne.nom_equipement, equipement, sizeof(ancienne.nom_equipement) - 1);
        ancienne.nom_equipement[sizeof(ancienne.nom_equipement) - 1] = '\0';
        
        // Convertir la quantité ancienne
        ancienne.quantite = atoi(quantite_str);
        
        strncpy(ancienne.salle, salle, sizeof(ancienne.salle) - 1);
        ancienne.salle[sizeof(ancienne.salle) - 1] = '\0';
        
        strncpy(ancienne.periode, periode, sizeof(ancienne.periode) - 1);
        ancienne.periode[sizeof(ancienne.periode) - 1] = '\0';
        
        // Parser la date ANCIENNE
        if (date) {
            if (sscanf(date, "%d/%d/%d", &ancienne.jour, &ancienne.mois, &ancienne.annee) != 3) {
                ancienne.jour = 1;
                ancienne.mois = 1;
                ancienne.annee = 2024;
            }
        }
        
        // Créer la structure NOUVELLE (copie de l'ancienne d'abord)
        ReservationEquipement nouvelle;
        memcpy(&nouvelle, &ancienne, sizeof(ReservationEquipement));
        
        // Récupérer les widgets de modification via lookup_widget
        GtkComboBox *combo_salle = GTK_COMBO_BOX(lookup_widget(window, "Changer_Salle"));
        GtkComboBox *combo_periode = GTK_COMBO_BOX(lookup_widget(window, "Changer_Periode"));
        GtkSpinButton *spin_quantite = GTK_SPIN_BUTTON(lookup_widget(window, "Modifier_Quantite"));
        GtkSpinButton *spin_jour = GTK_SPIN_BUTTON(lookup_widget(window, "jour"));
        GtkSpinButton *spin_mois = GTK_SPIN_BUTTON(lookup_widget(window, "mois"));
        GtkSpinButton *spin_annee = GTK_SPIN_BUTTON(lookup_widget(window, "annee"));
        
        // Vérifier que tous les widgets existent
        if (!combo_salle || !combo_periode || !spin_quantite || !spin_jour || !spin_mois || !spin_annee) {
            g_print("Erreur: Un ou plusieurs widgets de modification non trouvés\n");
            
            // Afficher message d'erreur dans le Label
            if (label_sortie) {
                gtk_label_set_text(label_sortie, "Erreur: widgets manquants");
            }
            
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_ERROR,
                                                      GTK_BUTTONS_OK,
                                                      "Erreur: Champs de modification non trouvés !");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            
            // Libérer la mémoire
            g_free(entraineur);
            g_free(type);
            g_free(equipement);
            g_free(quantite_str);
            g_free(salle);
            g_free(date);
            g_free(periode);
            return;
        }
        
        // Récupérer les NOUVELLES valeurs (avec GtkComboBox au lieu de GtkComboBoxText)
        gchar *nouvelle_salle = gtk_combo_box_get_active_text(combo_salle);
        gchar *nouvelle_periode = gtk_combo_box_get_active_text(combo_periode);
        int nouvelle_quantite = (int)gtk_spin_button_get_value(spin_quantite);
        int nouveau_jour = (int)gtk_spin_button_get_value(spin_jour);
        int nouveau_mois = (int)gtk_spin_button_get_value(spin_mois);
        int nouvelle_annee = (int)gtk_spin_button_get_value(spin_annee);
        
        // Vérifier que les champs ne sont pas vides
        if (!nouvelle_salle || !nouvelle_periode || 
            strlen(nouvelle_salle) == 0 || strlen(nouvelle_periode) == 0) {
            
            // Afficher message dans le Label
            if (label_sortie) {
                gtk_label_set_text(label_sortie, "Sélectionnez salle et période");
            }
            
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_WARNING,
                                                      GTK_BUTTONS_OK,
                                                      "Veuillez sélectionner une salle et une période !");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            
            // Libérer la mémoire
            g_free(entraineur);
            g_free(type);
            g_free(equipement);
            g_free(quantite_str);
            g_free(salle);
            g_free(date);
            g_free(periode);
            if (nouvelle_salle) g_free(nouvelle_salle);
            if (nouvelle_periode) g_free(nouvelle_periode);
            return;
        }
        
        // Mettre à jour la structure NOUVELLE avec sécurité
        strncpy(nouvelle.salle, nouvelle_salle, sizeof(nouvelle.salle) - 1);
        nouvelle.salle[sizeof(nouvelle.salle) - 1] = '\0';
        
        strncpy(nouvelle.periode, nouvelle_periode, sizeof(nouvelle.periode) - 1);
        nouvelle.periode[sizeof(nouvelle.periode) - 1] = '\0';
        
        nouvelle.quantite = nouvelle_quantite;
        nouvelle.jour = nouveau_jour;
        nouvelle.mois = nouveau_mois;
        nouvelle.annee = nouvelle_annee;
        
        // Demander confirmation
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_QUESTION,
                                                  GTK_BUTTONS_YES_NO,
                                                  "Voulez-vous vraiment modifier cette réservation ?\n\n"
                                                  "Entraîneur: %s\n"
                                                  "Équipement: %s\n\n"
                                                  "ANCIENNE: Salle %s, Période %s, Date %02d/%02d/%04d, Quantité %d\n"
                                                  "NOUVELLE: Salle %s, Période %s, Date %02d/%02d/%04d, Quantité %d",
                                                  entraineur, equipement,
                                                  ancienne.salle, ancienne.periode, 
                                                  ancienne.jour, ancienne.mois, ancienne.annee, ancienne.quantite,
                                                  nouvelle.salle, nouvelle.periode,
                                                  nouvelle.jour, nouvelle.mois, nouvelle.annee, nouvelle.quantite);
        
        gint result = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        if (result == GTK_RESPONSE_YES) {
            // Appeler la fonction de modification
            modifier_reservation(&ancienne, &nouvelle);
            
            // Rafraîchir l'affichage
            afficher_reservations_dans_treeview(treeview);
            
            // Afficher message de succès dans le Label
            if (label_sortie) {
                gtk_label_set_text(label_sortie, "Modification accomplie");
            }
            
            // Message de confirmation (dialogue optionnel)
            GtkWidget *info_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                           GTK_DIALOG_MODAL,
                                                           GTK_MESSAGE_INFO,
                                                           GTK_BUTTONS_OK,
                                                           "Réservation modifiée avec succès !");
            gtk_dialog_run(GTK_DIALOG(info_dialog));
            gtk_widget_destroy(info_dialog);
        }
        else {
            // Annulation
            if (label_sortie) {
                gtk_label_set_text(label_sortie, "Modification annulée");
            }
            
            GtkWidget *info_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                           GTK_DIALOG_MODAL,
                                                           GTK_MESSAGE_INFO,
                                                           GTK_BUTTONS_OK,
                                                           "Modification annulée.");
            gtk_dialog_run(GTK_DIALOG(info_dialog));
            gtk_widget_destroy(info_dialog);
        }
        
        // Libérer la mémoire
        g_free(entraineur);
        g_free(type);
        g_free(equipement);
        g_free(quantite_str);
        g_free(salle);
        g_free(date);
        g_free(periode);
        g_free(nouvelle_salle);
        g_free(nouvelle_periode);
    }
    else {
        // Aucune sélection
        if (label_sortie) {
            gtk_label_set_text(label_sortie, "Sélectionnez une réservation");
        }
        
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_WARNING,
                                                  GTK_BUTTONS_OK,
                                                  "Veuillez sélectionner une réservation à modifier !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// ============================================
// ALLER À L'INTERFACE ENTRAINEUR
// ============================================

void on_a_interfaceEntraineur_clicked  (GtkButton *button, gpointer user_data)
{   
    // Fermer TOUTES les fenêtres
    GList *list = gtk_window_list_toplevels();
    for (GList *l = list; l != NULL; l = l->next) {
        gtk_widget_hide(GTK_WIDGET(l->data));
    }
    g_list_free(list);
    
    // Créer nouvelle fenêtre
    GtkWidget *entraineur = create_Equippements_entraineurs();
    gtk_widget_show_all(entraineur);
}
