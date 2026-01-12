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
#include "year.h"                   // Include the Year struct definition
#include "month.h"                  // Include the Month struct definition
#include "day.h"                    // Include the Day struct definition
#include "task.h"                   // Include the Task struct definition
#include "helpers.h"                // Include the helper functions

// ===========================================================================================
// Function Implementations
// ===========================================================================================

/* AGENDA CREATE
alloceerd plaats voor de agenda (groodte van de struct agenda)
zet de root van agenda op null -> wordt later het eerste jaar van de agenda
geeft de agenda terug
*/
Agenda* agenda_create()
{
    Agenda* ag = malloc(sizeof(Agenda));
    ag->root = NULL;
    return ag;
}

/* AGENDA FREE
start het proces van het vrijgeven van heel de agenda
als er nog geen agenda is of de agenda is niet goed doorgegeven return
anders start het proces van alle jaren vrijgeven
zet de agenda terug vrij
*/
void agenda_free(Agenda* ag) 
{
    if (!ag) return;
    year_free(ag->root);
    free(ag);
}

/* AGENDA ADD TASK
heeft nodig:
- de agenda
- een datum (jaar maand dag)
- een taak struct

doet:
- zorgt dat het jaar de maand en de dag bestaan
- voegt de taak toe op de juiste dag
*/
void agenda_add_task(Agenda* agenda, int year, int month, int day, Task* task)
{
    agenda->root = year_insert(agenda->root, year);

    Year* y = year_find(agenda->root, year);
    Month* m = year_get_or_create_month(y, month);
    Day* d = month_get_or_create_day(m, day);

    day_add_task(d, task);
}


/* PRINT AGENDA
als er geen agenda is of de agenda heeft geen root, dan print agenda leeg
anders, print agenda contents gevolgd door het oproepen van het printen van alle jaren
gevolgd door een afsluit lijn
*/
void agenda_print(const Agenda* agenda) 
{
    if (!agenda || !agenda->root) {
        printf("(Agenda is empty)\n");
        return;
    }

    printf("=== AGENDA CONTENTS ===\n");
    year_print_tree(agenda->root);
    printf("=======================\n");
}

/* PRINT AGENDA DATE RANGE
heeft nodig:
- agenda
- start datum (jaar maand dag)
- eind datum (jaar maand dag)
doet:
- als er geen agenda is of de agenda heeft geen root, dan print agenda leeg
- anders print de agenda contents from <begin dag> <eind dag>
- gevolgd door de jaar print in range (geef beide data door)
- gevolgd door een eind lijn
*/
void agenda_print_date_range(const Agenda* agenda, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day) 
{
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

/* PRINT AGENDA KEYWORD
heeft nodig:
- agenda
- keyword
doet:
- als er geen agenda is of de agenda heeft geen root, dan print agenda leeg
- als er geen keyword is print de hele agenda
- als er wel een keyword is zoek het aantal matches
- als er 0 matches zijn print geen match gevonden
- als er wel matches zijn, print de matches
- print eindlijn
*/
void agenda_print_text_matching(const Agenda* agenda, const char* keyword) 
{
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

/* AGENDA DELETE DATUM RANGE
heeft nodig:
- agenda
- start datum
- eind datum
doet:
- als geen agenda of geen agenda root print lege agenda
-   als * startdatum is groter dan eind datum
    of  * startdatum = eind datum
    dan print invalid date range
- anders roep functie aan voor jaar datum delete tasks
*/
void agenda_delete_task_date_range(Agenda* agenda, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day)
{
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

/* AGENDA RUIM TAAK OP
als geen agenda of geen agenda root return (al klaar)
anders call de jaar functie voor alle taken opruimen
*/
void agenda_clear_tasks(Agenda* agenda) 
{
    if (!agenda || !agenda->root){
        return;
    }
    year_clear_all_tasks(agenda->root);
}

/* IMPORTING VAN FILE
-> filename
initialiseer filename met 256 characters
scanf filename en als een verkeerde input is print invalid input

-> content
call readfile functie op de filename (path)
als er geen content is print failed to read

-> loop voor lijnen

initialiseren:  * lijn nummer
                * lijn content
                * imported counter

de loop gaat dan over elke lijn in de filecontents (behalve de eerste) en
gerbuikt scanf om alle variabelen van een taak uit de lijn te halen en dan
wordt de taak toegevoegd aan de agenda met de eerder gebruikte add task functie
de trim whitespace helper functie wordt hier ook gebruikt voor de whitespaces in
de lijnen weg te halen
*/
void agenda_import_from_file(Agenda* agenda) {
    char filename[256];

    /* validated filename input */
    prompt_string("Enter the file to import: ", filename, sizeof(filename), "filename (e.g., data.txt)");

    char* file_content = read_file(filename);
    if (!file_content) {
        printf("Failed to read file: %s\n", filename);
        return;
    }

    int line_num = 0;
    char *line = strtok(file_content, "\n");
    int imported = 0;

    while (line) {
        if (line_num > 0) { 
            char *p = line;
            while (*p && isspace((unsigned char)*p)) p++;
            if (*p != '\0') {
                int year = 0, month = 0, day = 0;
                int sh = 0, sm = 0, eh = 0, em = 0;
                char title[100] = "";
                char description[300] = "";
                char location[100] = "";

                int matched = sscanf(line,
                    "%d-%d-%d, %d:%d - %d:%d | %99[^|] | %299[^|] | %99[^\n]",
                    &year, &month, &day, &sh, &sm, &eh, &em, title, description, location);

                if (matched == 10) {
                    trim_whitespace(title);
                    trim_whitespace(description);
                    trim_whitespace(location);

                    Task *t = task_create(sh, sm, eh, em, title, description, location);
                    agenda_add_task(agenda, year, month, day, t);
                    imported++;
                } else {
                    printf("Warning: could not parse line %d: %s\n", line_num + 1, line);
                }
            }
        }
        line_num++;
        line = strtok(NULL, "\n");
    }

    free(file_content);
    printf("Imported %d tasks from '%s'\n", imported, filename);
}

/* AGENDA EXPORT NAAR BESTAND
heeft nodig:
- agenda (pointer)
- filename (char aray pointer)

doet:
- maakt filehandeler met de filename
- als de filepointer niet lukt, geef error
- anders print agenda export op eerste lijn en ga dan naar 
    jaar tree functie voor het printen van de tasks naar een file
- op het einde file terug closen
*/
void agenda_export_to_file(Agenda* agenda, char* filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    fprintf(fp, "=== AGENDA EXPORT ===\n");
    year_print_tree_to_file(agenda->root, fp);

    fclose(fp);
}