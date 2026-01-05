// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: agenda.c                                                                           //
// Description: Implementation of the Agenda struct and related functions.                      //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================

#include <stdlib.h>                         // For malloc and free
#include <stdio.h>                          // For printf
#include <string.h>                         // For string functions
#include <ctype.h>                          // For tolower#include <stdbool.h>               // For bool
#include "agenda.h"                         // Include the Agenda struct definition
#include "Structs/year.h"                   // Include the Year struct definition
#include "Structs/month.h"                  // Include the Month struct definition
#include "Structs/day.h"                    // Include the Day struct definition
#include "Structs/task.h"                   // Include the Task struct definition
#include "import.h"
// ===========================================================================================
// Function Implementations
// ===========================================================================================

Agenda* agenda_create() {
    Agenda* ag = malloc(sizeof(Agenda));
    ag->root = NULL;
    return ag;
}

void agenda_free(Agenda* ag) {
    if (!ag) return;
    year_free(ag->root);
    free(ag);
}

void agenda_add_task(
    Agenda* agenda,
    int year,
    int month,
    int day,
    Task* task
) {
    // 1. Ensure YEAR exists
    agenda->root = year_insert(agenda->root, year);
    Year* y = year_find(agenda->root, year);

    // 2. Ensure MONTH exists inside that year
    Month* m = year_get_or_create_month(y, month);

    // 3. Ensure DAY exists inside that month
    Day* d = month_get_or_create_day(m, day);

    // 4. Add TASK to the day
    day_add_task(d, task);
}

void agenda_print(const Agenda* agenda) {
    if (!agenda || !agenda->root) {
        printf("(Agenda is empty)\n");
        return;
    }

    printf("=== AGENDA CONTENTS ===\n");
    year_print_tree(agenda->root);
    printf("=======================\n");
}

void agenda_print_date_range(
    const Agenda* agenda,
    int start_year,
    int start_month,
    int start_day,
    int end_year,
    int end_month,
    int end_day
) {
    if (!agenda || !agenda->root) {
        printf("(Agenda is empty)\n");
        return;
    }

    printf("=== AGENDA CONTENTS FROM %04d-%02d-%02d TO %04d-%02d-%02d ===\n",
           start_year, start_month, start_day,
           end_year, end_month, end_day);
    year_print_date_range(agenda->root, start_year, start_month, start_day, end_year, end_month, end_day);
    printf("=============================================================\n");
}

void agenda_print_text_matching(const Agenda* agenda, const char* keyword) {
    if (!agenda || !agenda->root) {
        printf("(Agenda is empty)\n");
        return;
    }

    if (!keyword || keyword[0] == '\0') {
        agenda_print(agenda);
        return;
    }

    int total = year_count_matches(agenda->root, keyword);
    printf("=== AGENDA ITEMS MATCHING '%s' ===\n", keyword);
    if (total == 0) {
        printf("(No matches found for '%s')\n", keyword);
    } else {
        year_print_matching(agenda->root, keyword);
    }
    printf("=================================\n");
}

void agenda_delete_task_date_range(Agenda* agenda, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day){
    if (!agenda || !agenda->root) {
        printf("(Agenda is empty)\n");
        return;
    }

    if (start_year > end_year || (start_year == end_year && (start_month > end_month || (start_month == end_month && start_day > end_day)))) {
        printf("(Invalid date range)\n");
        return;
    }

    year_delete_tasks_in_range(agenda->root, start_year, start_month, start_day, end_year, end_month, end_day);
}

void agenda_clear_tasks(Agenda* agenda) {
    if (!agenda || !agenda->root) return;
    year_clear_all_tasks(agenda->root);
}

void agenda_import_from_file(Agenda* agenda) {
    char filename[256];

    printf("Enter the JSON file to import: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid input\n");
        return;
    }

    char *json = read_file(filename);
    if (!json) {
        printf("Failed to read file\n");
        return;
    }

    const char *ptr = strstr(json, "\"agenda\": [");
    if (!ptr) { free(json); return; }
    ptr += 10; // move past "agenda":[

    agenda->root = year_parse(&ptr);
    free(json);
}

void agenda_export_to_file(Agenda* agenda) {
    FILE *fp = fopen("agenda.json", "w");
    if (!fp) {
        perror("Failed to open agenda.json");
        return;
    }

    fprintf(fp, "{ \"agenda\": ");
    if (agenda && agenda->root)
        year_print_tree_to_file(fp, agenda->root);
    else
        fprintf(fp, "[]");
    fprintf(fp, " }\n");

    fclose(fp);
}