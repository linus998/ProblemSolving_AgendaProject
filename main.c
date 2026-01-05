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
#include "Structs/task.h"

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
        printf("Enter your choice: ");              // input veld
        //=======================================
        scanf("%d", &choice);                       // scanf voor de gemaakte keuze
        
        switch (choice)                                        // switch voor de 5 keuzes van het main menu      
        {
            case 1: {                                           // case 1 (toevoegen van een taak)
                Task* t = malloc(sizeof(Task));                 // plaats alloceren voor 1 task
                if (!t) {                                       // test of de allocatie gelukt is
                    printf("Memory allocation failed!\n");      // print fout bootschap
                    break;                                      // terug naar == MAIN LOOP ==
                }

                int task_id = 1;                                // initialiseer de task id op 1
                t->id = task_id++;                              // task id zetten als task_id en dan auto increment
                t->next = NULL;                                 // volgende task in de gelinkte lijst init op NULL

                int year, month, day;                           // datum initialiseren
                printf("Enter year, month, day: ");             // vraag voor de datum
                scanf("%d %d %d", &year, &month, &day);         // scan de ingevoerde datum

                while (getchar() != '\n');                      // input buffer leegmaken

                printf("Enter task title: ");   
                fgets(t->title, sizeof(t->title), stdin);       // vraag titel en de size of voor dynamische geheugen allocatie
                t->title[strcspn(t->title, "\n")] = 0;          // verwijder newline

                printf("Enter task description: ");
                fgets(t->description, sizeof(t->description), stdin);   // vraag beschrijving en de size of voor dynamische geheugen allocatie
                t->description[strcspn(t->description, "\n")] = 0;      // verwijder newline

                printf("Enter task location: ");
                fgets(t->location, sizeof(t->location), stdin);     // vraag locatie en de size of voor dynamische geheugen allocatie
                t->location[strcspn(t->location, "\n")] = 0;        // verwijder newline

                printf("Enter start hour and minute: ");
                scanf("%d %d", &t->start_hour, &t->start_min);      // vraag start tijd
                printf("Enter end hour and minute: ");
                scanf("%d %d", &t->end_hour, &t->end_min);          // vraag eind tijd

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
                printf("Enter your choice: ");                  // input veld
                //=======================================
                scanf("%d", &printChoice);                      // scanf voor de gemaakte keuze

                switch (printChoice){           // == PRINT SWITCH == voor de 4 keuzes van het print menu
                    case 1:
                        agenda_print(agenda);   // keuze 1 (volledige print)
                        break;                  // naar einde == PRINT SWITCH ==
                    
                    case 2:                     // keuze 2 (print in datum range)
                        int start_year, start_month, start_day, end_year, end_month, end_day;   // initialiseer data
                        printf("Enter start year, month, day: ");                               // vraag start datum
                        scanf("%d %d %d", &start_year, &start_month, &start_day);               // scan start datum
                        printf("Enter end year, month, day: ");                                 // vraag eind datum
                        scanf("%d %d %d", &end_year, &end_month, &end_day);                     // scan eind datum

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
                        printf("Enter keyword to search: ");            
                        scanf("%s", keyword);                           
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
                printf("Enter your choice: ");              // input veld
                //=======================================
                scanf("%d", &deleteChoice);                 // scanf voor de gemaakte keuze

                switch (deleteChoice) // == DELETE SWITCH == voor de 3 keuzes van het delete menu
                {
                    case 1: // keuze 1 (delete in datum range)
                        int start_year, start_month, start_day, end_year, end_month, end_day;   // initialiseer data
                        printf("Enter start year, month, day: ");                               // vraag start datum  
                        scanf("%d %d %d", &start_year, &start_month, &start_day);               // scan start datum
                        printf("Enter end year, month, day: ");                                 // vraag eind datum 
                        scanf("%d %d %d", &end_year, &end_month, &end_day);                     // scan eind datum
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
                printf("Enter your choice: ");              // input veld
                //=======================================
                scanf("%d", &fileChoice);                   // scanf voor de gemaakte keuze

                switch (fileChoice) // == FILE IN OUT SWITCH == file in and output menu
                {

                case 1:                                 // keuze 1 (import van bestand)
                    agenda_import_from_file(agenda);    // functie call voor import
                    break;                              // naar einde == FILE IN OUT SWITCH ==
                
                case 2:                                         // keuze 2 (export naar bestand)
                    printf("File name to export to (.json): "); // vraag voor bestands naam
                    char filename[256];                         // initialiseer bestands naam array
                    scanf("%s", filename);                      // scan bestands naam
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