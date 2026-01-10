// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: month.h                                                                            //
// Description: Header file for the Month struct and related functions.                         //
// =============================================================================================//

// ===========================================================================================
// Header Guards
// ===========================================================================================
#ifndef MONTH_H
#define MONTH_H
typedef struct Day Day;         // Forward declaration of Day struct

// ===========================================================================================
// Struct Definition
// ===========================================================================================

typedef struct Month {      // Definition of the Month struct
    int month;              // Month number (1-12)
    Day* days;              // Pointer to the BST of days in the month

    struct Month* left;     // Pointer to the left child in the BST
    struct Month* right;    // Pointer to the right child in the BST
} Month;                    // End of Month struct definition

#include <stdio.h>

// ===========================================================================================
// Function Prototypes
// ===========================================================================================

Month* month_create(int month_num);
void month_free(Month* mt);
Month* month_insert(Month* root, int month_num);
Month* month_find(Month* root, int month_num);
void month_add_day(Month* month, Day* day);
void month_print_tree(Month* root);
Day* month_get_or_create_day(Month* month, int day_num);
void month_print_date_range(Month* root, int start_month, int start_day, int end_month, int end_day);
int month_count_matches(Month* month, const char* keyword);
void month_print_matching(Month* month, const char* keyword);
void month_delete_tasks_in_range(Month* root, int year, int start_month, int start_day, int end_month, int end_day, int start_year, int end_year);
void month_clear_all_tasks(Month* root);
void month_print_tree_to_file(Month* root, FILE* file, int year);

// ===========================================================================================
// end Header Guards
// ===========================================================================================
#endif