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
#include "contains.h"

// ===========================================================================================
// Function Implementations
// ===========================================================================================
Day* day_create(int day_num) {
    Day* dy = malloc(sizeof(Day));
    dy->day = day_num;
    dy->tasks = NULL;
    dy->left = NULL;
    dy->right = NULL;
    return dy;
}

void day_free(Day* dy) {
    if (!dy) return;
    task_free_all(dy->tasks);
    free(dy);
}

void day_free_all(Day* dy) {
    if (!dy) return;
    day_free_all(dy->left);
    day_free_all(dy->right);
    task_free(dy->tasks);
    free(dy);
}

Day* day_insert(Day* root, int day_num) {
    if (!root) return day_create(day_num);

    if (day_num < root->day)
        root->left = day_insert(root->left, day_num);
    else if (day_num > root->day)
        root->right = day_insert(root->right, day_num);

    return root;
}

Day* day_find(Day* root, int day_num) {
    if (!root) return NULL;

    if (day_num == root->day) return root;
    if (day_num < root->day) return day_find(root->left, day_num);
    return day_find(root->right, day_num);
}

void day_add_task(Day* day, Task* task) {
    task_add(&day->tasks, task);
}

void day_print_tree(Day* root) {
    if (!root) return;

    day_print_tree(root->left);

    printf("    Day %d\n", root->day);
    task_print_all(root->tasks);

    day_print_tree(root->right);
}

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

void day_delete_tasks_in_range(Day* root, int start_day, int end_day) {
    if (!root) return;
    day_delete_tasks_in_range(root->left, start_day, end_day);
    if (root->day >= start_day && root->day <= end_day) {
        task_free_all(root->tasks);
        root->tasks = NULL;
    }
    day_delete_tasks_in_range(root->right, start_day, end_day);
}

void day_clear_all_tasks(Day* root) {
    if (!root) return;
    day_clear_all_tasks(root->left);
    task_free_all(root->tasks);
    root->tasks = NULL;
    day_clear_all_tasks(root->right);
}

void day_print_tree_to_file(FILE *fp, Day *day) {
    fprintf(fp, "[");
    for (Day *d = day; d; d = d->right) {
        fprintf(fp, "{ \"day\": %d, \"tasks\": ", d->day);
        task_print_all_to_file(fp, d->tasks);
        fprintf(fp, " }%s", d->right ? ", " : "");
    }
    fprintf(fp, "]");
}