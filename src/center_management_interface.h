// center_management_interface.h
#ifndef CENTER_MANAGEMENT_INTERFACE_H
#define CENTER_MANAGEMENT_INTERFACE_H

#include <gtk/gtk.h>

// Structure to hold all data fields for a Sport Center
typedef struct {
    int id_center;          // N° Centre
    int id_room;            // N° Salle
    char name[100];         // Nom
    char address[100];      // Adresse
    char city[50];          // Ville
    char phone[20];         // Telephone
    int capacity;           // Capacite
    char schedules[50];     // Horaires
} SportCenter;


// Enumeration for GtkTreeView columns
enum {
    COL_ID_CENTER,
    COL_ID_ROOM,
    COL_NAME,
    COL_ADDRESS,
    COL_CITY,
    COL_PHONE,
    COL_CAPACITY,
    COL_SCHEDULES,
    NUM_COLS
};


// Interface Initialization
/** Initializes the GtkListStore model and sets up columns. */
void initialize_center_treeview(GtkWidget *treeview);


// Data Handling / CRUD Operations
/** Retrieves the iterator and path of the currently selected row. */
gboolean get_selected_iter_and_path(GtkWidget *treeview, GtkTreeIter *iter, GtkTreePath **path);


/** Reads data from GUI input fields and populates the SportCenter structure. */
void read_center_input_data(GtkBuilder *builder, SportCenter *center);

/** Adds a new SportCenter record to the GtkTreeView model. */
void add_center_to_treeview(GtkWidget *treeview, const SportCenter *center);

/** Implements simple filtering/searching in the GtkTreeView. */
void search_center_treeview(GtkWidget *treeview, const char *search_text);


// Callback Prototypes
void on_button41_clicked(GtkButton *button, gpointer user_data); // Ajouter (Add)
void on_button42_clicked(GtkButton *button, gpointer user_data); // Modifier (Modify)
void on_button43_clicked(GtkButton *button, gpointer user_data); // Supprimer (Delete)
void on_button40_clicked(GtkButton *button, gpointer user_data); // Rechercher (Search)
void on_Gestion_des_Centres_Sportifs_Admin_destroy(GtkObject *object, gpointer user_data); // Window close handler

#endif // CENTER_MANAGEMENT_INTERFACE_H
