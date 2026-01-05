// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: month.h                                                                            //
// Description: Header file for the Month struct and related functions.                         //
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
Month* month_create(int month_num) {
    Month* mn = malloc(sizeof(Month));
    mn->month = month_num;
    mn->days = NULL;
    mn->left = NULL;
    mn->right = NULL;
    return mn;
}

void month_free(Month* mn) {
    if (!mn) return;
    day_free(mn->days);
    free(mn);
}

void month_free_all(Month* mn) {
    if (!mn) return;
    month_free_all(mn->left);
    month_free_all(mn->right);
    day_free(mn->days);
    free(mn);
}

Month* month_insert(Month* root, int month_num) {
    if (!root) return month_create(month_num);

    if (month_num < root->month)
        root->left = month_insert(root->left, month_num);
    else if (month_num > root->month)
        root->right = month_insert(root->right, month_num);

    return root;
}

Month* month_find(Month* root, int month_num) {
    if (!root) return NULL;

    if (month_num == root->month) return root;
    if (month_num < root->month) return month_find(root->left, month_num);
    return month_find(root->right, month_num);
}

Day* month_get_or_create_day(Month* month, int day_num) {
    month->days = day_insert(month->days, day_num);
    return day_find(month->days, day_num);
}

void month_print_tree(Month* root) {
    if (!root) return;

    month_print_tree(root->left);

    printf("  Month %d\n", root->month);
    day_print_tree(root->days);

    month_print_tree(root->right);
}

void month_print_date_range(Month* root, int start_month, int start_day, int end_month, int end_day) {
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

int month_count_matches(Month* month, const char* keyword) {
    if (!month) return 0;
    int cnt = 0;
    cnt += month_count_matches(month->left, keyword);
    cnt += day_count_matches(month->days, keyword);
    cnt += month_count_matches(month->right, keyword);
    return cnt;
}

void month_print_matching(Month* month, const char* keyword) {
    if (!month) return;
    month_print_matching(month->left, keyword);
    if (day_count_matches(month->days, keyword) > 0) {
        printf("  Month %d\n", month->month);
        day_print_matching(month->days, keyword);
    }
    month_print_matching(month->right, keyword);
}

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

void month_clear_all_tasks(Month* root) {
    if (!root) return;
    month_clear_all_tasks(root->left);
    day_clear_all_tasks(root->days);
    month_clear_all_tasks(root->right);
}

void month_print_tree_to_file(FILE *fp, Month *month) {
    fprintf(fp, "[");
    for (Month *m = month; m; m = m->right) {
        fprintf(fp, "{ \"month\": %d, \"days\": ", m->month);
        day_print_tree_to_file(fp, m->days);
        fprintf(fp, " }%s", m->right ? ", " : "");
    }
    fprintf(fp, "]");
}