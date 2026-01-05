// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: task.h                                                                             //
// Description: Header file for the Task struct and related functions.                          //
// =============================================================================================//

// ===========================================================================================
// Header Guards
// ===========================================================================================
#ifndef TASK_H
#define TASK_H

// ===========================================================================================
// Struct Definition
// ===========================================================================================

typedef struct Task {           // Definition of the Task struct
    int id;                     // Unique identifier for the task (Auto-incremented)
    int start_hour;             // Start hour of the task
    int start_min;              // Start minute of the task
    int end_hour;               // End hour of the task
    int end_min;                // End minute of the task
    char title[100];            // Title of the task (dynamic data needed)
    char description[300];      // Description of the task (dynamic data needed)
    char location[100];         // Location of the task (dynamic data needed)
    struct Task* next;          // Pointer to the next task in the linked list
} Task;                         // End of Task struct definition

// ===========================================================================================
// Function Prototypes
// ===========================================================================================

Task* task_create(int id, int start_hour, int start_min, int end_hour, int end_min, const char* title, const char* description, const char* location);
void task_free(Task* tk);
void task_add(Task** head, Task* task);
void task_free_all(Task* tk);
void task_print_all(Task* task);
void task_print_all_to_file(FILE *fp, Task* task);

// ===========================================================================================
// end Header Guards
// ===========================================================================================
#endif