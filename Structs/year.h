// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: year.h                                                                             //
// Description: Header file for the Year struct and related functions.                          //
// =============================================================================================//

// ===========================================================================================
// Header Guards
// ===========================================================================================
#ifndef YEAR_H
#define YEAR_H
typedef struct Month Month;       // Forward declaration of Month struct

// ===========================================================================================
// Struct Definition
// ===========================================================================================

typedef struct year {       // Definition of the Year struct
    int year;               // Year number (e.g., 2023)
    Month* months;          // Pointer to the BST of months in the year

    struct year* left;      // Pointer to the left child in the BST
    struct year* right;     // Pointer to the right child in the BST
} Year;                     // End of Year struct definition

// ===========================================================================================
// Function Prototypes
// ===========================================================================================

Year* year_create(int year_num);
void year_free(Year* yr);
void year_free_all(Year* yr);
Year* year_insert(Year* root, int year_num);
Year* year_find(Year* root, int year_num);
Month* year_get_or_create_month(Year* year, int month_num);
void year_print_tree(Year* root);
void year_print_date_range(Year* root, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day);
int year_count_matches(Year* year, const char* keyword);
void year_print_matching(Year* year, const char* keyword);
void year_delete_tasks_in_range(Year* root, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day);
void year_clear_all_tasks(Year* root);
void year_print_tree_to_file(FILE *fp, Year* root);

// ===========================================================================================
// end Header Guards
// ===========================================================================================
#endif