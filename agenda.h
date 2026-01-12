// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: agenda.h                                                                           //
// Description: Header file for the Agenda struct and related functions.                        //
// =============================================================================================//

// ===========================================================================================
// Header Guards
// ===========================================================================================
#ifndef AGENDA_H
#define AGENDA_H
#include "task.h"
#include "day.h"
#include "month.h"
#include "year.h"
#include "helpers.h"

// ===========================================================================================
// Struct Definition
// ===========================================================================================

typedef struct Agenda {         // Agenda struct definition
    Year* root;                 // Pointer to the root year node
} Agenda;                       // End of Agenda struct definition

// ===========================================================================================
// Function Prototypes
// ===========================================================================================
Agenda* agenda_create(void);
void agenda_free(Agenda* ag);
void agenda_add_task(Agenda* agenda,int year,int month,int day,Task* task);
void agenda_print(const Agenda* agenda);
void agenda_print_date_range(const Agenda* agenda, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day);
void agenda_print_text_matching(const Agenda* agenda, const char* keyword);
void agenda_delete_task_date_range(Agenda* agenda, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day);
void agenda_clear_tasks(Agenda* agenda);
void agenda_import_from_file(Agenda* agenda);
void agenda_export_to_file(Agenda* agenda, char* filename);

// ===========================================================================================
// end of header guards
// ===========================================================================================
#endif

