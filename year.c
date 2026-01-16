// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: year.c                                                                             //
// Description: Implementation file for the Year struct and related functions.                  //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "year.h"
#include "month.h"

// ===========================================================================================
// Function Implementations
// ===========================================================================================
/* MAAK NIEUW JAAR
alloceer struct size
vul jaar nummer in
de rest NULL en return het jaar
*/
Year* year_create(int year_num) 
{
    Year* yr = malloc(sizeof(Year));
    yr->year = year_num;
    yr->months = NULL;
    yr->left = NULL;
    yr->right = NULL;
    return yr;
}

/* VERWIJDER JAAR
als jaar return
anders verwijder jaar functie
verwijder jaar zelf
*/
void year_free(Year* yr) 
{
    if (!yr) return;
    month_free(yr->months);
    free(yr);
}

/* ALLE JAREN VERWIJDEREN
als geen jaar, return
recursief links en rechts jaren verwijderen
verwijder alle maanden in het originele jaar
verwijder het jaar zelf
*/
void year_free_all(Year* yr) 
{
    if (!yr) return;
    year_free_all(yr->left);
    year_free_all(yr->right);
    month_free(yr->months);
    free(yr);
}

/* JAAR INSERTEN
als geen root return het jaar door create new year
als jaar kleiner is dan rootn de linker van de root zet gelijk aan 
het jaar dat gemaakt wordt door recursief de functie aan te roepen
anders als het jaar groter is, zelfde maar dan rechts
anders gewoon root terug geven
*/
Year* year_insert(Year* root, int year_num) 
{
    if (!root) return year_create(year_num);

    if (year_num < root->year)
        root->left = year_insert(root->left, year_num);
    else if (year_num > root->year)
        root->right = year_insert(root->right, year_num);

    return root;
}

/* YEAR FIND
als er geen root is return NUll
als het jaarnummer hetzelfde is als de root, return de root
als jaarnummer kleiner is, recursief links zoeken
als het jaarnummer groter is (al de rest) recursief rechts zoeken
*/
Year* year_find(Year* root, int year_num) 
{
    if (!root) return NULL;

    if (year_num == root->year) return root;
    if (year_num < root->year) return year_find(root->left, year_num);
    return year_find(root->right, year_num);
}

/* YEAR GET OR CREATE MONTH
probeer insert te doen op maand
geef de maand terug door month find
*/
Month* year_get_or_create_month(Year* year, int month_num) 
{
    year->months = month_insert(year->months, month_num);
    return month_find(year->months, month_num);
}

/* JAAR PINT BOOM
als er geen root is return
jaar print tree left recursief
print jaar tekst
print de month tree
print jaar tree rechts recursief
*/
void year_print_tree(Year* root) 
{
    if (!root) return;

    year_print_tree(root->left);

    printf("Year %d\n", root->year);
    month_print_tree(root->months);

    year_print_tree(root->right);
}

/* JAAR PRINT DATE RANGE
heeft nodig: 
- root, start datum (jaar maand dag) eind datum (jaar maand dag)
doet:
als er geen root is return
als de root year groter is dan de start year, recursief links printen
als de root groter of gelijk is aan start year, en de root year is kleiner of gelijk aan eind jaar
month print date range met gegeven data
anders (root year is kleiner dan eind jaar), print recursief rechts
*/
void year_print_date_range(
    Year* root,
    int start_year,
    int start_month,
    int start_day,
    int end_year,
    int end_month,
    int end_day
) 
{
    if (!root) return;

    if (root->year > start_year)
        year_print_date_range(root->left, start_year, start_month, start_day, end_year, end_month, end_day);

    if (root->year >= start_year && root->year <= end_year) {
        month_print_date_range(
            root->months,
            (root->year == start_year) ? start_month : 1,
            (root->year == start_year) ? start_day : 1,
            (root->year == end_year) ? end_month : 12,
            (root->year == end_year) ? end_day : 31
        );
    }

    if (root->year < end_year)
        year_print_date_range(root->right, start_year, start_month, start_day, end_year, end_month, end_day);
}

/* JAAR TEL MACHES
als er geen jaar is return 0
anders init cnt variabel als 0
count recursief links 
count maanden in dit jaar
count recursief rechts
return de count
*/
int year_count_matches(Year* year, const char* keyword) 
{
    if (!year) return 0;
    int cnt = 0;
    cnt += year_count_matches(year->left, keyword);
    cnt += month_count_matches(year->months, keyword);
    cnt += year_count_matches(year->right, keyword);
    return cnt;
}

/* JAAR PRINT MACHES
als geen jaar return
anders print recursief links
als er maanden zijn met de keyword (count month maches)
print het jaar in tekst, en print de maanden
print recursief rechts
*/
void year_print_matching(Year* year, const char* keyword) {
    if (!year) return;
    year_print_matching(year->left, keyword);
    if (month_count_matches(year->months, keyword) > 0) {
        printf("Year %d\n", year->year);
        month_print_matching(year->months, keyword);
    }
    year_print_matching(year->right, keyword);
}

/* YEAR DELETE TASKS IN RANGE
als er geen root is return
anders, als root year groter is dan start year, print recursief links
als jaar groter of gelijk is aan start jaar en kleiner of gelijk aan eind jaar
init start maand en eind maand, en call moth delete tasks in range met die data
als de root kleiner is dan eind jaar, print year right recursief
*/
void year_delete_tasks_in_range(Year* root, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day) {
    if (!root) return;

    if (root->year > start_year)
        year_delete_tasks_in_range(root->left, start_year, start_month, start_day, end_year, end_month, end_day);

    if (root->year >= start_year && root->year <= end_year) {
        int ms = (root->year == start_year) ? start_month : 1;
        int me = (root->year == end_year) ? end_month : 12;
        month_delete_tasks_in_range(root->months, root->year, ms, start_day, me, end_day, start_year, end_year);
    }

    if (root->year < end_year)
        year_delete_tasks_in_range(root->right, start_year, start_month, start_day, end_year, end_month, end_day);
}

/*YEAR CLEAR ALL TASKS
als er geen root is return
anders clear recursief links
clear de maanden van dit jaar
clear recursief rechts
*/
void year_clear_all_tasks(Year* root) 
{
    if (!root) return;
    year_clear_all_tasks(root->left);
    month_clear_all_tasks(root->months);
    year_clear_all_tasks(root->right);
}

/* JAAR PRINT TO FILE
als er geen root is, return
anders print recursief links 
print moth tree to file
print recursief rechts
*/
void year_print_tree_to_file(Year* root, FILE* file) {
    if (!root) return;

    year_print_tree_to_file(root->left, file);
    month_print_tree_to_file(root->months, file, root->year);

    year_print_tree_to_file(root->right, file);
    
} 