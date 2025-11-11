// sport_center_data_manager.c
// Functions for managing Sport Center data using basic file I/O.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For rename() and remove()

#define DATABASE_FILE "sport_centers.txt"
#define TEMP_FILE "sport_centers_temp.txt"

// Data structure for a Sport Center record in the file
typedef struct {
    int id_center;          // N° Centre
    char name[100];         // Nom
    char city[50];          // Ville
    int capacity;           // Capacite
} SportCenter;


// 1. ADD DATA
/** Appends a new SportCenter record to the text file. */
int add_center_to_file(const SportCenter *center) {
    // Open the file in append mode ('a')
    FILE *file = fopen(DATABASE_FILE, "a");
    
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not open file %s for writing.\n", DATABASE_FILE);
        return 0;
    }

    // Write data using ';' delimiter
    fprintf(file, "%d;%s;%s;%d\n", 
            center->id_center,
            center->name,
            center->city,
            center->capacity);

    fclose(file);
    return 1;
}


// 2. READ DATA
/** Reads all SportCenter records from the file. */
int display_all_centers() {
    // Open the file in read mode ('r')
    FILE *file = fopen(DATABASE_FILE, "r");
    SportCenter current_center;
    int count = 0;

    if (file == NULL) {
        printf("Warning: Database file not found. No centers to display.\n");
        return 0;
    }

    // Read data separated by ';' (4 fields expected)
    while (fscanf(file, "%d;%99[^;];%49[^;];%d\n", 
                  ¤t_center.id_center,
                  current_center.name,
                  current_center.city,
                  ¤t_center.capacity) == 4) 
    {
        count++;
    }
    
    fclose(file);
    return count;
}


// 3. UPDATE DATA
/** Updates a SportCenter record based on the ID (Copy-and-Replace method). */
int update_center_in_file(const SportCenter *center) {
    FILE *original_file = fopen(DATABASE_FILE, "r");
    FILE *temp_file = fopen(TEMP_FILE, "w");
    SportCenter current_center;
    int found = 0;

    if (original_file == NULL || temp_file == NULL) {
        fprintf(stderr, "ERROR: Could not open files for update operation.\n");
        if (original_file) fclose(original_file);
        if (temp_file) fclose(temp_file);
        return 0;
    }

    while (fscanf(original_file, "%d;%99[^;];%49[^;];%d\n", 
                  ¤t_center.id_center,
                  current_center.name,
                  current_center.city,
                  ¤t_center.capacity) == 4) 
    {
        if (current_center.id_center == center->id_center) {
            // Write the NEW data
            fprintf(temp_file, "%d;%s;%s;%d\n", 
                    center->id_center,
                    center->name,
                    center->city,
                    center->capacity);
            found = 1;
        } else {
            // Write the OLD data back
            fprintf(temp_file, "%d;%s;%s;%d\n", 
                    current_center.id_center,
                    current_center.name,
                    current_center.city,
                    current_center.capacity);
        }
    }

    fclose(original_file);
    fclose(temp_file);

    if (found) {
        // Replace original file
        remove(DATABASE_FILE);
        rename(TEMP_FILE, DATABASE_FILE);
        return 1;
    } else {
        // Cleanup temp file
        remove(TEMP_FILE);
        return 0;
    }
}


// 4. DELETE DATA
/** Deletes a SportCenter record based on the ID (Copy-and-Replace method). */
int delete_center_from_file(int center_id) {
    FILE *original_file = fopen(DATABASE_FILE, "r");
    FILE *temp_file = fopen(TEMP_FILE, "w");
    SportCenter current_center;
    int found = 0;

    if (original_file == NULL || temp_file == NULL) {
        fprintf(stderr, "ERROR: Could not open files for delete operation.\n");
        if (original_file) fclose(original_file);
        if (temp_file) fclose(temp_file);
        return 0;
    }

    while (fscanf(original_file, "%d;%99[^;];%49[^;];%d\n", 
                  ¤t_center.id_center,
                  current_center.name,
                  current_center.city,
                  ¤t_center.capacity) == 4) 
    {
        if (current_center.id_center == center_id) {
            // Skip writing this record (deletion)
            found = 1; 
        } else {
            // Keep the record: write OLD data
            fprintf(temp_file, "%d;%s;%s;%d\n", 
                    current_center.id_center,
                    current_center.name,
                    current_center.city,
                    current_center.capacity);
        }
    }

    fclose(original_file);
    fclose(temp_file);

    if (found) {
        // Replace original file
        remove(DATABASE_FILE);
        rename(TEMP_FILE, DATABASE_FILE);
        return 1;
    } else {
        // Cleanup temp file
        remove(TEMP_FILE);
        return 0;
    }
}
