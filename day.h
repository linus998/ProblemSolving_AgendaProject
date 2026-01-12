// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: day.h                                                                              //
// Description: Header file for the Day struct and related functions.                           //
// =============================================================================================//

// ===========================================================================================
// Header Guards
// ===========================================================================================
#ifndef DAY_H               
#define DAY_H

typedef struct Task Task;   // Forward declaration of Task struct

// ===========================================================================================
// Struct Definition
// ===========================================================================================

typedef struct Day {        // Definition of the Day struct
    int day;                // Day of the month
    Task* tasks;            // Pointer to the linked list of tasks for the day

    struct Day* left;       // Pointer to the left child in the BST
    struct Day* right;      // Pointer to the right child in the BST
} Day;

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdio.h>

// ===========================================================================================
// Function Prototypes
// ===========================================================================================

Day* day_create(int day_num);
void day_free(Day* dy);
void day_free_all(Day* dy);
Day* day_insert(Day* root, int day_num);
Day* day_find(Day* root, int day_num);
void day_add_task(Day* day, Task* task);
void day_print_tree(Day* root);
void day_print_date_range(Day* root, int start_day, int end_day);
int day_count_matches(Day* day, const char* keyword);
void day_print_matching(Day* day, const char* keyword);
void day_delete_tasks_in_range(Day* root, int start_day, int end_day);
void day_clear_all_tasks(Day* root);
void day_print_tree_to_file(Day* root, FILE* file, int year, int month);

// ===========================================================================================
// end Header Guards
// ===========================================================================================
#endif