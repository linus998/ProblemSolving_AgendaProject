// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: task.c                                                                             //
// Description: Implementation file for the Task struct and related functions.                  //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "task.h"

// task id counter
static int task_id = 0;                        
                           
// ===========================================================================================
// Function Implementations
// ===========================================================================================

/* TASK CREATE
alloceer de grootte van een task struct
task id +  (static int in deze file)
alle members van de task struct invullen met de doorgegeven argumenten
return de task
*/
Task *task_create(int start_hour, int start_min, int end_hour, int end_min, const char *title, const char *description, const char *location)
{   
    Task *tk = malloc(sizeof(Task));
    tk->id = task_id++;
    tk->start_hour = start_hour;
    tk->start_min = start_min;
    tk->end_hour = end_hour;
    tk->end_min = end_min;
    snprintf(tk->title, sizeof(tk->title), "%s", title);
    snprintf(tk->description, sizeof(tk->description), "%s", description);
    snprintf(tk->location, sizeof(tk->location), "%s", location);
    tk->next = NULL;
    return tk;
}

/* VERWIJDER TASK   
als er geen taak is return
anders free de task
*/
void task_free(Task *tk)
{
    if (!tk)
        return;
    free(tk);
}

/* VOEG TAAK TOE
head van de gelinkte lijst, nieuwe head naar de task
*/
void task_add(Task **head, Task *task)
{
    task->next = *head;
    *head = task;
}

/* TASK FREE ALL
als geen task, return
anders recursief de hele gelinkte lijst verwijderen
en dan de taak zelf verwijderen
*/
void task_free_all(Task *tk)
{
    if (!tk)
        return;
    task_free_all(tk->next);
    free(tk);
}

/* TASK PRINT ALL
zolang er een taak is print de taak en ga naar de volgende
*/
void task_print_all(Task *task)
{
    while (task)
    {
        printf("    [Task %d] %02d:%02d - %02d:%02d | %s | %s | %s\n", task->id, task->start_hour, task->start_min, task->end_hour, task->end_min, task->title, task->description, task->location);
        task = task->next;
    }
}

/* PRINT TO FILE
zelfde als print all maar naar file en ander formaat voor export
*/
void task_print_all_to_file(Task *task, FILE *file, int year, int month, int day)
{
    while (task)
    {
        fprintf(file, "%04d-%02d-%02d, %02d:%02d - %02d:%02d | %s | %s | %s\n", year, month, day, task->start_hour, task->start_min, task->end_hour, task->end_min, task->title, task->description, task->location);
        task = task->next;
    }
}