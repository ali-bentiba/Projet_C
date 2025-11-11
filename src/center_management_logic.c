// center_management_logic.c

#include "center_management_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Interface Initialization

void initialize_center_treeview(GtkWidget *treeview)
{
    GtkListStore  *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Create a new GtkListStore model with defined column types
    store = gtk_list_store_new(NUM_COLS, 
                               G_TYPE_INT,   // COL_ID_CENTER
                               G_TYPE_INT,   // COL_ID_ROOM
                               G_TYPE_STRING, // COL_NAME
                               G_TYPE_STRING, // COL_ADDRESS
                               G_TYPE_STRING, // COL_CITY
                               G_TYPE_STRING, // COL_PHONE
                               G_TYPE_INT,   // COL_CAPACITY
                               G_TYPE_STRING // COL_SCHEDULES
                              );
    
    // Attach the model to the treeview
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store); 

    // Macro for column creation
    #define CREATE_COLUMN(title, col_id) \
        renderer = gtk_cell_renderer_text_new(); \
        column = gtk_tree_view_column_new_with_attributes(title, \
                                                          renderer, \
                                                          "text", col_id, \
                                                          NULL); \
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Column Setup
    
    CREATE_COLUMN("N° Centre", COL_ID_CENTER);
    CREATE_COLUMN("N° Salle", COL_ID_ROOM);
    CREATE_COLUMN("Nom", COL_NAME);
    CREATE_COLUMN("Adresse", COL_ADDRESS);
    CREATE_COLUMN("Ville", COL_CITY);
    CREATE_COLUMN("Telephone", COL_PHONE);
    CREATE_COLUMN("Capacite", COL_CAPACITY);
    CREATE_COLUMN("Horaires", COL_SCHEDULES);

    // Make the last column sortable
    gtk_tree_view_column_set_sort_column_id(column, COL_SCHEDULES); 
    
    #undef CREATE_COLUMN
}


// Data Handling/CRUD Operations

// Helper to get selected row iterator and path
gboolean get_selected_iter_and_path(GtkWidget *treeview, GtkTreeIter *iter, GtkTreePath **path)
{
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    
    // Get the first selected row
    if (gtk_tree_selection_get_selected(selection, &model, iter)) {
        // Get the path (caller must free it)
        *path = gtk_tree_model_get_path(model, iter); 
        return TRUE;
    }
    return FALSE;
}


void read_center_input_data(GtkBuilder *builder, SportCenter *center)
{
    // Retrieve GUI widgets
    GtkWidget *entry_name = GTK_WIDGET(gtk_builder_get_object(builder, "entry17"));
    GtkWidget *entry_address = GTK_WIDGET(gtk_builder_get_object(builder, "entry18"));
    GtkWidget *entry_city = GTK_WIDGET(gtk_builder_get_object(builder, "entry19"));
    GtkWidget *entry_phone = GTK_WIDGET(gtk_builder_get_object(builder, "entry20"));
    GtkWidget *spin_capacity = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton9"));
    GtkWidget *entry_schedules = GTK_WIDGET(gtk_builder_get_object(builder, "entry21"));
    GtkWidget *spin_id_center = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton41"));
    GtkWidget *spin_id_room = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton42"));

    // Get values from widgets
    const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
    const char *address = gtk_entry_get_text(GTK_ENTRY(entry_address));
    const char *city = gtk_entry_get_text(GTK_ENTRY(entry_city));
    const char *phone = gtk_entry_get_text(GTK_ENTRY(entry_phone));
    const char *schedules = gtk_entry_get_text(GTK_ENTRY(entry_schedules));
    
    center->capacity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_capacity));
    center->id_center = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_id_center));
    center->id_room = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_id_room));

    // Copy string data safely to the struct
    strncpy(center->name, name, sizeof(center->name) - 1);
    center->name[sizeof(center->name) - 1] = '\0';
    
    strncpy(center->address, address, sizeof(center->address) - 1);
    center->address[sizeof(center->address) - 1] = '\0';

    strncpy(center->city, city, sizeof(center->city) - 1);
    center->city[sizeof(center->city) - 1] = '\0';

    strncpy(center->phone, phone, sizeof(center->phone) - 1);
    center->phone[sizeof(center->phone) - 1] = '\0';

    strncpy(center->schedules, schedules, sizeof(center->schedules) - 1);
    center->schedules[sizeof(center->schedules) - 1] = '\0';
}

void add_center_to_treeview(GtkWidget *treeview, const SportCenter *center)
{
    GtkListStore *store;
    GtkTreeIter iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    
    if (!store) {
        g_warning("Could not get the ListStore model from treeview5.");
        return;
    }

    gtk_list_store_append(store, &iter);

    // Set the values in the ListStore row
    gtk_list_store_set(store, &iter,
                       COL_ID_CENTER, center->id_center,
                       COL_ID_ROOM, center->id_room, 
                       COL_NAME, center->name,
                       COL_ADDRESS, center->address,
                       COL_CITY, center->city,
                       COL_PHONE, center->phone,
                       COL_CAPACITY, center->capacity,
                       COL_SCHEDULES, center->schedules,
                       -1); 
}

void search_center_treeview(GtkWidget *treeview, const char *search_text)
{
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    GtkTreeModelFilter *filter = NULL;

    // Get or create the filter model
    if (GTK_IS_TREE_MODEL_FILTER(model)) {
        filter = GTK_TREE_MODEL_FILTER(model);
        model = gtk_tree_model_filter_get_model(filter);
    } else {
        filter = GTK_TREE_MODEL_FILTER(gtk_tree_model_filter_new(model, NULL));
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(filter));
    }
    
    // If search text is empty, revert to the original model
    if (search_text == NULL || search_text[0] == '\0') {
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model); 
        g_object_unref(filter); 
        return;
    }

    // For this simple example, we revert to the original model (no filter applied)
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model); 
}


// Callback Implementations

void on_button41_clicked(GtkButton *button, gpointer user_data) 
{
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, "treeview5"));
    SportCenter new_center;
    
    // 1. Read data from the form
    read_center_input_data(builder, &new_center);

    // 2. Validate data
    if (strlen(new_center.name) == 0) {
        g_warning("Validation failed: Center Name (Nom) cannot be empty.");
        // TODO: Show an error message box to the user instead of printing to console.
        return;
    }

    // 3. Add the data to the GtkListStore
    add_center_to_treeview(treeview, &new_center);
    // TODO: Add logic to save the center to the persistent file/DB using add_center_to_file
}

void on_button42_clicked(GtkButton *button, gpointer user_data) 
{
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, "treeview5"));
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

    GtkTreeIter iter;
    GtkTreePath *path = NULL;
    SportCenter updated_center;
    
    // 1. Check if a row is selected
    if (!get_selected_iter_and_path(treeview, &iter, &path)) {
        g_warning("Modification failed: No center selected.");
        // TODO: Show an error message box
        return;
    }
    
    // 2. Read updated data from the form
    read_center_input_data(builder, &updated_center);

    // 3. Validate data
    if (strlen(updated_center.name) == 0) {
        g_warning("Validation failed: Center Name (Nom) cannot be empty for update.");
        // TODO: Show an error message box
        if (path) gtk_tree_path_free(path);
        return;
    }

    // 4. Update the record in the GtkListStore model
    gtk_list_store_set(store, &iter,
                       COL_ID_CENTER, updated_center.id_center,
                       COL_ID_ROOM, updated_center.id_room, 
                       COL_NAME, updated_center.name,
                       COL_ADDRESS, updated_center.address,
                       COL_CITY, updated_center.city,
                       COL_PHONE, updated_center.phone,
                       COL_CAPACITY, updated_center.capacity,
                       COL_SCHEDULES, updated_center.schedules,
                       -1); 

    g_message("Center updated successfully in the ListStore.");
    if (path) gtk_tree_path_free(path); // Free the path
    
    // TODO: Implement logic to update the record in the persistent file/DB using update_center_in_file
}

void on_button43_clicked(GtkButton *button, gpointer user_data) 
{
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, "treeview5"));
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

    GtkTreeIter iter;
    GtkTreePath *path = NULL;
    int center_id_to_delete = 0;

    // 1. Check if a row is selected
    if (!get_selected_iter_and_path(treeview, &iter, &path)) {
        g_warning("Deletion failed: No center selected.");
        // TODO: Show an error message box
        return;
    }
    
    // Get the ID of the selected center (Needed for DB/File deletion)
    gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, COL_ID_CENTER, ¢er_id_to_delete, -1);
    
    // 2. Remove the selected row from the GtkListStore model
    if (gtk_list_store_remove(store, &iter)) {
        g_message("Center deleted successfully from the ListStore (ID: %d).", center_id_to_delete);
        // TODO: Implement logic to delete the record from the persistent file/DB using delete_center_from_file
    } else {
        g_warning("Failed to delete center from ListStore.");
    }

    if (path) gtk_tree_path_free(path); // Free the path
}

void on_button40_clicked(GtkButton *button, gpointer user_data) 
{
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *treeview = GTK_WIDGET(gtk_builder_get_object(builder, "treeview5"));
    GtkWidget *entry_search = GTK_WIDGET(gtk_builder_get_object(builder, "entry16"));
    
    const char *search_text = gtk_entry_get_text(GTK_ENTRY(entry_search));

    // Perform the search/filtering operation
    search_center_treeview(treeview, search_text);
}

void on_Gestion_des_Centres_Sportifs_Admin_destroy(GtkObject *object, gpointer user_data)
{
    // Quit the Gtk main loop
    gtk_main_quit();
}
