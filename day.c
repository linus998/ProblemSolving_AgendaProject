// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: day.c                                                                              //
// Description: Implementation file for the Day struct and related functions.                   //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "day.h"
#include "task.h"
#include "helpers.h"

// ===========================================================================================
// Function Implementations
// ===========================================================================================

/* DAG MAKEN
maakt plaats voor de day struct
assign het dag nummer en al de rest op NULL
geef de dag terug
*/
Day* day_create(int day_num)
{
    Day* dy = malloc(sizeof(Day));
    dy->day = day_num;
    dy->tasks = NULL;
    dy->left = NULL;
    dy->right = NULL;
    return dy;
}

/* DAG VERWIJDEREN
als er geen dag meegegeven is return,
anders verwijder alle taken van die dag en bevrijd dan ook de dag zelf 
*/
void day_free(Day* dy) 
{
    if (!dy) return;
    task_free_all(dy->tasks);
    free(dy);
}

/* VERWIJDER ALLE DAGEN
verwijderd alle dagenn in een maand (recursief)
als er geen dagen zijn return en anders verwijder de dagen
links en rechts recursief, en verwijder alle taken van alle dagen
en dan free de originele dag
*/
void day_free_all(Day* dy) {
    if (!dy) return;
    day_free_all(dy->left);
    day_free_all(dy->right);
    task_free(dy->tasks);
    free(dy);
}

/* DAG MAKEN ALS NIET BESTAAT
als er nog geen root is in die maand, maak de root die dag
als de nieuwe dag kliener is dan de root maak de dag links aan
als de nieuwe dag groter is dan de root maak de dag rechts aan
return de root
*/
Day* day_insert(Day* root, int day_num) {
    if (!root) return day_create(day_num);

    if (day_num < root->day)
        root->left = day_insert(root->left, day_num);
    else if (day_num > root->day)
        root->right = day_insert(root->right, day_num);

    return root;
}

/* VIND EEN BEPAALDE DAG
als er geen root is return NULL
anders als de dag gelijk is aan de root geef de dag terug mee
anders als de dag kleiner is zek recursief de dag aan de linker kan
en als geen van die waar is betekend dat dat het rechts zit
-> zoek recursief rechts de dag rechts
*/
Day* day_find(Day* root, int day_num) {
    if (!root) return NULL;
    if (day_num == root->day) return root;
    if (day_num < root->day) return day_find(root->left, day_num);
    return day_find(root->right, day_num);
}

/* VOEG TAAK TOE AAN DAG
call task_add met de juiste dag van de taak en de taak
*/
void day_add_task(Day* day, Task* task) {
    task_add(&day->tasks, task);
}

/* PRINT DE BOOM (dag)
als er geen root is return
anders:
print alles links (eerdere dagen)
print de dag zelf
print alles rechts (latere dagen)
*/
void day_print_tree(Day* root) {
    if (!root) return;

    day_print_tree(root->left);

    printf("    Day %d\n", root->day);
    task_print_all(root->tasks);

    day_print_tree(root->right);
}

/* PRINT DAGEN IN DATE RANGE
als geen root return
als de dag van de root groter is dan de startdag
print recursief de linker dag
als de root dag groter of gelijk is aan startdatum (dag) print de dag zelf
als de root dag kleiner is dan de eind datum (dag) print rechter dag 
*/
void day_print_date_range(Day* root, int start_day, int end_day) {
    if (!root) return;

    if (root->day > start_day)
        day_print_date_range(root->left, start_day, end_day);

    if (root->day >= start_day && root->day <= end_day) {
        printf("    Day %d\n", root->day);
        task_print_all(root->tasks);
    }

    if (root->day < end_day)
        day_print_date_range(root->right, start_day, end_day);
}
/* TEL DE MATCHENDE DAGEN (use helper contains)
als er geen dag is return 0
anders initialiseer een count integer (cnt)
recursief de cnt increasen door de dag links en rechts ook te checken
voor elke taak in de dag die de keyword contains cnt ++
zelfde recursief naar rechts
return count
*/
int day_count_matches(Day* day, const char* keyword) {
    if (!day) return 0;
    int cnt = 0;
    cnt += day_count_matches(day->left, keyword);
    for (Task* cur = day->tasks; cur; cur = cur->next) {
        if (contains_ci(cur->title, keyword) || contains_ci(cur->description, keyword) || contains_ci(cur->location, keyword))
            cnt++;
    }
    cnt += day_count_matches(day->right, keyword);
    return cnt;
}

/* DAG PRINT MATCHING KEYWORD (use helper contains)
als geen dag return
anders print machting recursief links
initialiseer int day_matching counter
voor elke taak in de dag, als die het keyword contaains or in de
discription het keyword zit, print die taak
(als het een nieuwe dag is print ook de dag)
en ook elke keer couter ++
*/
void day_print_matching(Day* day, const char* keyword) {
    if (!day) return;
    day_print_matching(day->left, keyword);
    int day_matches = 0;
    for (Task* cur = day->tasks; cur; cur = cur->next) {
        if (contains_ci(cur->title, keyword) || contains_ci(cur->description, keyword) || contains_ci(cur->location, keyword)) {
            if (!day_matches) {
                printf("    Day %d\n", day->day);
            }
            printf(
                "      [Task %d] %02d:%02d - %02d:%02d | %s | %s | %s\n",
                cur->id,
                cur->start_hour, cur->start_min,
                cur->end_hour, cur->end_min,
                cur->title,
                cur->description,
                cur->location
            );
            day_matches++;
        }
    }
    day_print_matching(day->right, keyword);
}

/* DELETE TAKEN IN DATUM RANGE
als er geen root is return
recursief delete tasks left
als dag binne de range, tasks free all van die dag
free de root van die dag
recursief rechts checken
*/
void day_delete_tasks_in_range(Day* root, int start_day, int end_day) {
    if (!root) return;
    day_delete_tasks_in_range(root->left, start_day, end_day);
    if (root->day >= start_day && root->day <= end_day) {
        task_free_all(root->tasks);
        root->tasks = NULL;
    }
    day_delete_tasks_in_range(root->right, start_day, end_day);
}

/* DAG CLEAR TAKEN (alle dagen van 1 maand)
als geen root return
anders recursief links en rechts en alle taken van de
originele dag vrijgeven
*/
void day_clear_all_tasks(Day* root) {
    if (!root) return;
    day_clear_all_tasks(root->left);
    task_free_all(root->tasks);
    root->tasks = NULL;
    day_clear_all_tasks(root->right);
}

/* EXPORT TO FILE DAG
als geen root return
anders print dag links print dag rechts recursief 
en print alle taken van die dag
*/
void day_print_tree_to_file(Day* root, FILE* file, int year, int month) {
    if (!root) return;

    day_print_tree_to_file(root->left, file, year, month);

    task_print_all_to_file(root->tasks, file, year, month, root->day);

    day_print_tree_to_file(root->right, file, year, month);
}