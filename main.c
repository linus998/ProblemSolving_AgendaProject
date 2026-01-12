// ===================================-- H O O F D I N G --=====================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: main.c                                                                             //
// Description: Main programma voor de agenda applicaie, bevat command line menu's              //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agenda.h"
#include "task.h"
#include "helpers.h"

// ===========================================================================================
// Main Function
// ===========================================================================================
int main() {
    Agenda* agenda = agenda_create();           // Nieuwe agenda aanmaken
    int choice;                                 // menu keuze
    int printChoice;                            // print menu keuze
    int deleteChoice;                           // delete menu keuze
    int fileChoice;                             // keuze voor import export menu

    do {                                            // == MAIN LOOP ==
        // Main menu
        //=======================================
        printf("\n=== AGENDA MENU ===\n");          // seperator
        printf("1. Add a task\n");                  // keuze 1, taak toevoegen
        printf("2. View tasks\n");                  // keuze 2, taken bekijken (meerdere mogelijkheden)
        printf("3. Delete a task\n");               // keuze 3, verwijderen van taken (meerdere mogelijkheden)
        printf("4. File in and output\n");          // keuze 4, input and output van de agenda (meerdere mogelijkheden)
        printf("5. Exit\n");                        // keuze 5, exit
        /* prompt and validate main menu choice */
        choice = prompt_int("Enter your choice: ", "integer (e.g., 1)");                       // input with validation
        
        switch (choice)                                        // switch voor de 5 keuzes van het main menu      
        {
            case 1: {                                           // case 1 (toevoegen van een taak)
                int year, month, day;                           // datum initialiseren
                /* validated date input */
                prompt_three_ints("Enter year, month, day (YYYY MM DD): ", &year, &month, &day, "YYYY MM DD (e.g., 2026 1 31)");

                char title[100];                                // initialiseer title array
                char description[300];                          // initialiseer description array
                char location[100];                             // initialiseer location array

                printf("Enter task title: ");
                fgets(title, sizeof(title), stdin);             // vraag en scan de titel
                title[strcspn(title, "\n")] = 0;                // verwijder newline karakter

                printf("Enter task description: ");
                fgets(description, sizeof(description), stdin); // vraag en scan de beschrijving
                description[strcspn(description, "\n")] = 0;    // verwijder newline karakter

                printf("Enter task location: ");
                fgets(location, sizeof(location), stdin);       // vraag en scan de locatie
                location[strcspn(location, "\n")] = 0;          // verwijder newline karakter

                int start_hour, start_min, end_hour, end_min;   // initialiseer tijd variabelen
                prompt_two_ints("Enter start hour and minute (HH MM): ", &start_hour, &start_min, "HH MM (e.g., 9 30)");
                prompt_two_ints("Enter end hour and minute (HH MM): ", &end_hour, &end_min, "HH MM (e.g., 10 30)");

                Task* t = task_create(start_hour, start_min, end_hour, end_min, title, description, location);  // maak nieuwe taak met alle ingevoerde data
                agenda_add_task(agenda, year, month, day, t);   // taak toevoegen aan de agenda met alle ingevoegde data

                printf("Task added!\n");                        // bevestiging van toegevoegde taak
                break;                                          // terug naar == MAIN LOOP ==
            }
            case 2:
                // print menu
                //=======================================
                printf("\n=== PRINT MENU ===\n");
                printf("1. Print agenda\n");                    // keuze 1, print de volledige agenda
                printf("2. Print tasks in date range\n");       // keuze 2, print (bepaalde datum range)
                printf("3. Print text matching\n");             // keuze 3, print (bepaalde tekst bevatten)
                printf("4. Exit\n");                            // keuze 4, exit
                printChoice = prompt_int("Enter your choice: ", "integer (e.g., 1)");                      // validated print menu choice

                switch (printChoice){           // == PRINT SWITCH == voor de 4 keuzes van het print menu
                    case 1:
                        agenda_print(agenda);   // keuze 1 (volledige print)
                        break;                  // naar einde == PRINT SWITCH ==
                    
                    case 2:                     // keuze 2 (print in datum range)
                        int start_year, start_month, start_day, end_year, end_month, end_day;   // initialiseer data
                        prompt_three_ints("Enter start year, month, day (YYYY MM DD): ", &start_year, &start_month, &start_day, "YYYY MM DD (e.g., 2026 1 31)");
                        prompt_three_ints("Enter end year, month, day (YYYY MM DD): ", &end_year, &end_month, &end_day, "YYYY MM DD (e.g., 2026 2 28)");

                        agenda_print_date_range(    // print functie callen    
                            agenda, 
                            start_year, 
                            start_month, 
                            start_day, 
                            end_year, 
                            end_month, 
                            end_day
                        );
                        break;                      // naar einde == PRINT SWITCH ==
                    
                    case 3:                                             // keuze 3 (print met bepaalde tekst)
                        char keyword[100];                              // initialiseer keyword array
                        prompt_string("Enter keyword to search: ", keyword, sizeof(keyword), "single word (no spaces)");
                        agenda_print_text_matching(agenda, keyword);    // print functie callen met text matching
                        break;                                          // naar einde == PRINT SWITCH ==
                        
                    case 4:                     // keuze 4 (exit)
                        break;                  // naar einde == PRINT SWITCH ==

                    default:                                                    // default case (ongeldige keuze)
                        printf("Invalid choice. Returning to main menu.\n");    // fout boodschap printen
                        break;                                                  // naar einde == PRINT SWITCH ==
                }
                break;  // terug naar == MAIN LOOP ==

            case 3:
                // delete menu
                //=======================================
                printf("\n=== DELETE MENU ===\n");
                printf("1. Delete task in date range\n");   // keuze 1, delete in datum range
                printf("2. Clear all tasks\n");             // keuze 2, delete alle taken
                printf("3. Exit\n");                        // keuze 3, exit
                deleteChoice = prompt_int("Enter your choice: ", "integer (e.g., 1)");                 // validated delete choice

                switch (deleteChoice) // == DELETE SWITCH == voor de 3 keuzes van het delete menu
                {
                    case 1: // keuze 1 (delete in datum range)
                        int start_year, start_month, start_day, end_year, end_month, end_day;   // initialiseer data
                        prompt_three_ints("Enter start year, month, day (YYYY MM DD): ", &start_year, &start_month, &start_day, "YYYY MM DD (e.g., 2026 1 31)");
                        prompt_three_ints("Enter end year, month, day (YYYY MM DD): ", &end_year, &end_month, &end_day, "YYYY MM DD (e.g., 2026 2 28)");
                        agenda_delete_task_date_range(  //delete funtion callen met date range
                            agenda,
                            start_year, 
                            start_month, 
                            start_day, 
                            end_year, 
                            end_month, 
                            end_day
                        );

                        break; // naar einde == DELETE SWITCH ==

                    case 2:                     // keuze 2 (delete alle taken)
                        agenda_free(agenda);    // functie call free heel de agenda
                        break;                  // naar einde == DELETE SWITCH ==

                    case 3:            // keuze 3 (exit)
                        break;         // naar einde == DELETE SWITCH ==

                    default:                                                    // default case (ongeldige keuze)
                        printf("Invalid choice. Returning to main menu.\n");    // fout boodschap printen
                        break;                                                  // naar einde == DELETE SWITCH ==
                }
                break; // terug naar == MAIN LOOP ==

            case 4:
                // file in and output menu
                //=======================================
                printf("\n=== FILE MENU ===\n");
                printf("1. Import from file\n");            // keuze 1, import van bestand
                printf("2. Export to file\n");              // keuze 2, export naar bestand
                printf("3. Exit\n");                        // keuze 3, exit
                fileChoice = prompt_int("Enter your choice: ", "integer (e.g., 1)");                   // validated file menu choice

                switch (fileChoice) // == FILE IN OUT SWITCH == file in and output menu
                {

                case 1:                                 // keuze 1 (import van bestand)
                    agenda_import_from_file(agenda);    // functie call voor import
                    break;                              // naar einde == FILE IN OUT SWITCH ==
                
                case 2:                                         // keuze 2 (export naar bestand)
                    char filename[256];                         // initialiseer bestands naam array
                    prompt_string("File name to export to (without extension): ", filename, sizeof(filename) - 5, "filename without extension (e.g., backup)");
                    strcat(filename, ".txt");                   // voeg .txt extensie toe aan bestands naam
                    agenda_export_to_file(agenda, filename);    // functie call voor export
                    break;                                      // naar einde == FILE IN OUT SWITCH ==

                default:                                // default case (ongeldige keuze)
                    break;                              // naar einde == FILE IN OUT SWITCH ==
                }
                break;                                  // terug naar == MAIN LOOP ==

            case 5:                         // keuze 5(main menu)(exit)
                printf("Exiting...\n");     // exit boodschap printen
                break;                      // naar einde == MAIN LOOP ==

            default:                                    // default case (ongeldige keuze)
                printf("Invalid choice. Try again.\n");  // fout boodschap printen
        }
    } while (choice != 5);  // blijf in de main loop totdat keuze 5 (exit) is gekozen

    agenda_free(agenda); // vrijgeven van alle geheugen gebruikt door de agenda
    return 0;            // einde van main functie
}