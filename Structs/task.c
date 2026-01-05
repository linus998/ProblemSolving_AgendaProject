// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: task.c                                                                              //
// Description: Implementation file for the Task struct and related functions.                   //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "task.h"

// ===========================================================================================
// Function Implementations
// ===========================================================================================
Task *task_create(int id, int start_hour, int start_min, int end_hour, int end_min, const char *title, const char *description, const char *location)
{
    Task *tk = malloc(sizeof(Task));
    tk->id = id;
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

void task_free(Task *tk)
{
    if (!tk)
        return;
    free(tk);
}

void task_add(Task **head, Task *task)
{
    task->next = *head;
    *head = task;
}

void task_free_all(Task *tk)
{
    if (!tk)
        return;
    task_free_all(tk->next);
    free(tk);
}

void task_print_all_to_file(FILE *fp, Task *task)
{
    fprintf(fp, "[");
    for (Task *t = task; t; t = t->next) {
        fprintf(fp,
            "{ \"id\": %d, \"start\": \"%02d:%02d\", \"end\": \"%02d:%02d\", "
            "\"title\": \"%s\", \"description\": \"%s\", \"location\": \"%s\" }%s",
            t->id, t->start_hour, t->start_min, t->end_hour, t->end_min,
            t->title, t->description, t->location,
            t->next ? ", " : ""  // comma only if not last
        );
    }
    fprintf(fp, "]");
}

void task_print_all(Task *task)
{
    while (task)
    {
        printf("    [Task %d] %02d:%02d - %02d:%02d | %s | %s | %s\n", task->id, task->start_hour, task->start_min, task->end_hour, task->end_min, task->title, task->description, task->location);
        task = task->next;
    }
}