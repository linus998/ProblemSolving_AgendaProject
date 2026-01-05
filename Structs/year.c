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
Year* year_create(int year_num) {
    Year* yr = malloc(sizeof(Year));
    yr->year = year_num;
    yr->months = NULL;
    yr->left = NULL;
    yr->right = NULL;
    return yr;
}

void year_free(Year* yr) {
    if (!yr) return;
    month_free(yr->months);
    free(yr);
}

void year_free_all(Year* yr) {
    if (!yr) return;
    year_free_all(yr->left);
    year_free_all(yr->right);
    month_free(yr->months);
    free(yr);
}

Year* year_insert(Year* root, int year_num) {
    if (!root) return year_create(year_num);

    if (year_num < root->year)
        root->left = year_insert(root->left, year_num);
    else if (year_num > root->year)
        root->right = year_insert(root->right, year_num);

    return root;
}

Year* year_find(Year* root, int year_num) {
    if (!root) return NULL;

    if (year_num == root->year) return root;
    if (year_num < root->year) return year_find(root->left, year_num);
    return year_find(root->right, year_num);
}

Month* year_get_or_create_month(Year* year, int month_num) {
    year->months = month_insert(year->months, month_num);
    return month_find(year->months, month_num);
}

void year_print_tree(Year* root) {
    if (!root) return;

    year_print_tree(root->left);

    printf("Year %d\n", root->year);
    month_print_tree(root->months);

    year_print_tree(root->right);
}

void year_print_date_range(
    Year* root,
    int start_year,
    int start_month,
    int start_day,
    int end_year,
    int end_month,
    int end_day
) {
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

int year_count_matches(Year* year, const char* keyword) {
    if (!year) return 0;
    int cnt = 0;
    cnt += year_count_matches(year->left, keyword);
    cnt += month_count_matches(year->months, keyword);
    cnt += year_count_matches(year->right, keyword);
    return cnt;
}

void year_print_matching(Year* year, const char* keyword) {
    if (!year) return;
    year_print_matching(year->left, keyword);
    if (month_count_matches(year->months, keyword) > 0) {
        printf("Year %d\n", year->year);
        month_print_matching(year->months, keyword);
    }
    year_print_matching(year->right, keyword);
}

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

void year_clear_all_tasks(Year* root) {
    if (!root) return;
    year_clear_all_tasks(root->left);
    month_clear_all_tasks(root->months);
    year_clear_all_tasks(root->right);
}

void year_print_tree_to_file(FILE *fp, Year *year) {
    fprintf(fp, "[");
    for (Year *y = year; y; y = y->right) {
        printf("DEBUG: Printing year %d to file\n", y->year);
        fprintf(fp, "{ \"year\": %d, \"months\": ", y->year);
        month_print_tree_to_file(fp, y->months);
        fprintf(fp, " }%s", y->right ? ", " : "");
    }
    fprintf(fp, "]");
}