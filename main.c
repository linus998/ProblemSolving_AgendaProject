// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: main.c                                                                             //
// Description: Main program for the agenda application.                                        //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agenda.h"
#include "Structs/task.h"

// ===========================================================================================
// Main Function
// ===========================================================================================
int main() {
    Agenda* agenda = agenda_create();
    int choice;
    int printChoice;
    int deleteChoice;
    int fileChoice;

    do {
        // Display menu
        printf("\n=== AGENDA MENU ===\n");
        printf("1. Add a task\n");
        printf("2. View tasks\n");
        printf("3. Delete a task\n");
        printf("4. File in and output\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                Task* t = malloc(sizeof(Task));
                if (!t) {
                    printf("Memory allocation failed!\n");
                    break;
                }

                static int task_id = 1;
                t->id = task_id++;
                t->next = NULL;

                int year, month, day;
                printf("Enter year, month, day: ");
                scanf("%d %d %d", &year, &month, &day);

                while (getchar() != '\n');

                printf("Enter task title: ");
                fgets(t->title, sizeof(t->title), stdin);
                t->title[strcspn(t->title, "\n")] = 0;

                printf("Enter task description: ");
                fgets(t->description, sizeof(t->description), stdin);
                t->description[strcspn(t->description, "\n")] = 0;

                printf("Enter task location: ");
                fgets(t->location, sizeof(t->location), stdin);
                t->location[strcspn(t->location, "\n")] = 0;

                printf("Enter start hour and minute: ");
                scanf("%d %d", &t->start_hour, &t->start_min);
                printf("Enter end hour and minute: ");
                scanf("%d %d", &t->end_hour, &t->end_min);

                agenda_add_task(agenda, year, month, day, t);

                printf("Task added!\n");
                break;
            }
            case 2:
                printf("\n=== PRINT MENU ===\n");
                printf("1. Print agenda\n");
                printf("2. Print tasks in date range\n");
                printf("3. Print text matching\n");
                printf("4. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &printChoice);

                switch (printChoice){
                    case 1:
                        agenda_print(agenda);
                        break;
                    
                    case 2:
                        int start_year, start_month, start_day, end_year, end_month, end_day;
                        printf("Enter start year, month, day: ");
                        scanf("%d %d %d", &start_year, &start_month, &start_day);
                        printf("Enter end year, month, day: ");
                        scanf("%d %d %d", &end_year, &end_month, &end_day);
                        agenda_print_date_range(agenda, start_year, start_month, start_day, end_year, end_month, end_day);
                        break;
                    
                    case 3:
                        char keyword[100];
                        printf("Enter keyword to search: ");
                        scanf("%s", keyword);
                        agenda_print_text_matching(agenda, keyword);
                        break;

                    default:
                        printf("Invalid choice. Returning to main menu.\n");
                        break;
                }
                break;

            case 3:
                printf("\n=== DELETE MENU ===\n");
                printf("1. Delete task in date range\n");
                printf("2. Clear all tasks\n");
                printf("3. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &deleteChoice);

                switch (deleteChoice) {
                    case 1:
                        int start_year, start_month, start_day, end_year, end_month, end_day;
                        printf("Enter start year, month, day: ");
                        scanf("%d %d %d", &start_year, &start_month, &start_day);
                        printf("Enter end year, month, day: ");
                        scanf("%d %d %d", &end_year, &end_month, &end_day);
                        agenda_delete_task_date_range(agenda, start_year, start_month, start_day, end_year, end_month, end_day);
                        break;

                    case 2:
                        agenda_free(agenda);
                        break;

                    case 3:
                        break;

                    default:
                        printf("Invalid choice. Returning to main menu.\n");
                        break;
                }
                break;
            case 4:

                printf("\n=== FILE MENU ===\n");
                printf("1. Import from file\n");
                printf("2. Export to file\n");
                printf("3. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &fileChoice);

                switch (fileChoice)
                {
                case 1:
                    // agenda_import_from_file(agenda);
                    break;
                
                case 2:
                    agenda_export_to_file(agenda);
                    break;

                default:
                    break;
                }
                
                break;

            case 5:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    // Free memory
    agenda_free(agenda);
    return 0;
}
