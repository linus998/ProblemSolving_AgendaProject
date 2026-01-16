// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: month.c                                                                            //
// Description: File to implement the Month struct and related functions.                       //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "month.h"
#include "day.h"

// ===========================================================================================
// Function Implementations
// ===========================================================================================

/* CREATE MONTH
aloceer de groodte van een month strcut
maand nummer -> binnen een jaar
maand days -> de root dag van die maand
maand left -> de linker maand (voor traversing de boom)
maand rechts -> de rechter maand (voor traversing de boom)
return de maand
*/
Month* month_create(int month_num) 
{
    Month* mn = malloc(sizeof(Month));
    mn->month = month_num;
    mn->days = NULL;
    mn->left = NULL;
    mn->right = NULL;
    return mn;
}

/* VERWIJDERE EEN MAAND
als er geen maand is return
anders verwijder alle dagen binnen de maand
en verwijder de maand
*/
void month_free(Month* mn) 
{
    if (!mn) return;
    day_free(mn->days);
    free(mn);
}

/* MONTH FREE ALL
als geen maand return
anders recursief alle maanden links en rechts verwijderen
en dan de dag van die maand zelf verwijderen
en dan de maand pointer verwijderen
*/
void month_free_all(Month* mn) 
{
    if (!mn) return;
    month_free_all(mn->left);
    month_free_all(mn->right);
    day_free(mn->days);
    free(mn);
}

/* INSERT MONTH
als er geen root is moet je de maand maken
als het maandnummer minder is dan de root, recursief naar links gaan
als het maandnummer meer is dan de root, recursief naar rechts gaan
anders (het is dezelfde maand) -> gewoon de root terug geven
*/
Month* month_insert(Month* root, int month_num) 
{
    if (!root) return month_create(month_num);

    if (month_num < root->month)
        root->left = month_insert(root->left, month_num);
    else if (month_num > root->month)
        root->right = month_insert(root->right, month_num);

    return root;
}

/* MONTH FIND
als er geen root is return NULL
anders als het maandnummer hetzelfde is return root
als het maandnummer kleiner is, recursief naar links zoeken
als het maandnummer groter is, recursief naar rechts zoeken
*/
Month* month_find(Month* root, int month_num) 
{
    if (!root) return NULL;

    if (month_num == root->month) return root;
    if (month_num < root->month) return month_find(root->left, month_num);
    return month_find(root->right, month_num);
}

/* MONTH GET OR CREATE DAY
insert de dag ( als die al bestaat word die gewoon gereturned)
geef de dag terug door de day_find te gebruiken
*/
Day* month_get_or_create_day(Month* month, int day_num) 
{
    month->days = day_insert(month->days, day_num);
    return day_find(month->days, day_num);
}

/* MONTH PRINT TREE
als er geen root is return
recursief linker maand printen (juiste volgorde)
dan de maand zelf als text uitprinten
dan de dagen van de maand zelf uitprinten
recursief rechter maand printen (juiste volgorde)
*/
void month_print_tree(Month* root) 
{
    if (!root) return;

    month_print_tree(root->left);

    printf("  Month %d\n", root->month);
    day_print_tree(root->days);

    month_print_tree(root->right);
}

/* MONTH PRINT DATE RANGE
zelfde als de gewone month print tree maar geeft telkens start datum (dag maand) en eind datum (dag maand) mee
aan de day_print_date_range enkel de start dag en eind dag meegeven
*/
void month_print_date_range(Month* root, int start_month, int start_day, int end_month, int end_day) 
{
    if (!root) return;

    if (root->month > start_month)
        month_print_date_range(root->left, start_month, start_day, end_month, end_day);

    if (root->month >= start_month && root->month <= end_month) {
        printf("  Month %d\n", root->month);
        day_print_date_range(root->days, (root->month == start_month) ? start_day : 1,
                             (root->month == end_month) ? end_day : 31);
    }

    if (root->month < end_month)
        month_print_date_range(root->right, start_month, start_day, end_month, end_day);
}

/* MONTH COUNT MATCHES
als er geen maand is return 0
anders init een cnt variabele op 0
dan recursief links en rechts de maanden tellen en dan de dagen tellen in deze maand
dan als laatste de dagen cnt returenen
*/
int month_count_matches(Month* month, const char* keyword) 
{
    if (!month) return 0;
    int cnt = 0;
    cnt += month_count_matches(month->left, keyword);
    cnt += day_count_matches(month->days, keyword);
    cnt += month_count_matches(month->right, keyword);
    return cnt;
}

/* MONTH PRINT MACHING
als er geen maand is return
anders recursief links en recht printen en als er dagen zijn in die maand met maching, 
dan print de maand en print de machende dagen
*/
void month_print_matching(Month* month, const char* keyword) 
{
    if (!month) return;
    month_print_matching(month->left, keyword);
    if (day_count_matches(month->days, keyword) > 0) {
        printf("  Month %d\n", month->month);
        day_print_matching(month->days, keyword);
    }
    month_print_matching(month->right, keyword);
}

/* MONTH DELETE TASKS IN RANGE
-als er geen root is return
-als de maand van de root groter is dan de startmaand,
delete de taken links recursief
-als de maand groter of gelijk aan is, init 2 variabkes for start day en end day en
call de day delete tasks in range functie met die data
-als de root maand kleiner is dan de eind maand, recursief rechts verwijderen 
*/
void month_delete_tasks_in_range(Month* root, int year, int start_month, int start_day, int end_month, int end_day, int start_year, int end_year) {
    if (!root) return;
    if (root->month > start_month)
        month_delete_tasks_in_range(root->left, year, start_month, start_day, end_month, end_day, start_year, end_year);

    if (root->month >= start_month && root->month <= end_month) {
        int ds = (year == start_year && root->month == start_month) ? start_day : 1;
        int de = (year == end_year && root->month == end_month) ? end_day : 31;
        day_delete_tasks_in_range(root->days, ds, de);
    }

    if (root->month < end_month)
        month_delete_tasks_in_range(root->right, year, start_month, start_day, end_month, end_day, start_year, end_year);
}

/* CLEAR ALL TAKS
als er geen root is return
anders recursief links en recht verwijderen en de dagen uit de maand zelf verwijderen
*/
void month_clear_all_tasks(Month* root) {
    if (!root) return;
    month_clear_all_tasks(root->left);
    day_clear_all_tasks(root->days);
    month_clear_all_tasks(root->right);
}

/* MONTH PRINT TREE TO FILE
zelfde als gewone print maar zonder de print van de maand in tekst
*/
void month_print_tree_to_file(Month* root, FILE* file, int year) {
    if (!root) return;

    month_print_tree_to_file(root->left, file, year);

    day_print_tree_to_file(root->days, file, year, root->month);

    month_print_tree_to_file(root->right, file, year);

}