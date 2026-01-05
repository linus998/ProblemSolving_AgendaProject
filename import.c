// ===================================-- H O O F D I N G --=====================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: import.h                                                                           //
// Description: import en export functies en prasers                                            //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================

// C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Structs
#include "Structs/day.h"
#include "Structs/task.h"
#include "Structs/month.h"
#include "Structs/year.h"

// ===========================================================================================
// Functie implementaties
// ===========================================================================================


char* read_file(const char *filename)       // Helper voor het lezen van de files
{
    FILE *fp = fopen(filename, "r");        // file pointer
    if (!fp) return NULL;                   // error check

    fseek(fp, 0, SEEK_END);                 // ga naar het einde van de file
    long size = ftell(fp);                  // get file size
    rewind(fp);                             // terug naar het begin

    char *buffer = malloc(size + 1);            // allocate memory voor file content
    if (!buffer) { fclose(fp); return NULL; }   // error check

    fread(buffer, 1, size, fp);             // lees bestand in in buffer
    buffer[size] = '\0';                    // null-terminate string
    fclose(fp);                             // sluit file   
    return buffer;                          // geef de buffer met inhoud van de file terug
}

void parse_string(const char* src, char* dest, int max_len) // Helper voor sting na colom uit te lezen
{
    const char *colon = strchr(src, ':');       // zoek dubbele punt
    if (!colon) { dest[0] = '\0'; return; }     // Foutafhandeling geen dubbele punt
    const char *start = strchr(colon, '"');     // zoek eerste aanhalingsteken na de dubbele punt
    if (!start) { dest[0] = '\0'; return; }     // Foutafhandeling geen aanhalingsteken
    start++;                                    // zoek eerste karakter van de waarde
    const char *end = strchr(start, '"');       // zoek sluitende aanhalingsteken
    if (!end) { dest[0] = '\0'; return; }       // Foutafhandeling geen sluitend aanhalingsteken
    int len = end - start;                      // Lengte van de waarde
    if (len >= max_len) len = max_len - 1;      // buffer size managen
    strncpy(dest, start, len);                  // zet waarde ook in dest
    dest[len] = '\0';                           // null-terminate dest string
}

int parse_int(const char *src)                  // Helper om integer na dubbele punt uit te lezen
{ 
    const char *colon = strchr(src, ':');       // zoek dubbele punt
    if (!colon) return 0;                       // Foutafhandeling geen dubbele punt
    return strtol(colon + 1, NULL, 10);         // substring na dubbele punt naar integer
}

Task* task_parse(const char** ptr)                                      // Helper om taken array te parsen           
{
    Task *head = NULL;                                                  // Hoofd van de gelinkte lijst
    while (**ptr && **ptr != ']') {                                     // Zolang in de array
        
        const char *id_str = strstr(*ptr, "\"id\":");                   // kijken voor "id": n
        if (!id_str) break;                                             // geen id -> stop met parsen    
        int id = parse_int(id_str);                                     // parse id

        // start en eind time
        // ---------------------------------------------------------------------------------------------------------
        int sh = 0, sm = 0, eh = 0, em = 0;                             // start hour, start min, end hour, end min
        char s[6] = {0}, e[6] = {0};                                    // buffers voor strings

        const char *start_str = strstr(*ptr, "\"start\":");             // kijken voor "start": "hh:mm"
        if (start_str) {                                                // als er een start tijd is
            parse_string(start_str, s, sizeof(s));                      // parse de string   
            sscanf(s, "%d:%d", &sh, &sm);                               // split in uren en minuten
        }

        const char *end_str = strstr(*ptr, "\"end\":");                 // kijken voor "end": "hh:mm"
        if (end_str) {                                                  // als er een eind tijd is
            parse_string(end_str, e, sizeof(e));                        // parse de string
            sscanf(e, "%d:%d", &eh, &em);                               // split in uren en minuten
        }

        // title, description, location
        // ---------------------------------------------------------------------------------------------------------
        char title[128] = "", desc[256] = "", loc[128] = "";            // buffers voor strings

        const char *t_str = strstr(*ptr, "\"title\":");                 // kijken voor "title":
        if (t_str) parse_string(t_str, title, sizeof(title));           // parse de title

        const char *d_str = strstr(*ptr, "\"description\":");           // kijken voor "description":
        if (d_str) parse_string(d_str, desc, sizeof(desc));             // parse de description

        const char *l_str = strstr(*ptr, "\"location\":");              // kijken voor "location":
        if (l_str) parse_string(l_str, loc, sizeof(loc));               // parse de location

        // aanmaken en toevoegen van de taak
        // ---------------------------------------------------------------------------------------------------------
        Task *t = task_create(id, sh, sm, eh, em, title, desc, loc);    // maak nieuwe taak aan
        task_add(&head, t);                                             // voeg taak toe aan gelinkte lijst

        const char *next = strchr(*ptr, '}');  // volgende taak zoeken
        if (!next) break;                      // geen volgende taak -> stop met parsen
        *ptr = next + 1;                       // verplaats pointer naar na de volgende taak
    }
    return head; // geef hoofd van de gelinkte lijst door
}


Day* day_parse(const char** ptr)                                // Helper om dagen array te parsen
{
    Day *root = NULL;                                           // Root van de BST         
    while (**ptr && **ptr != ']') {                             // Zolang in de array
        const char *day_str = strstr(*ptr, "\"day\":");         // kijken voor "day": n
        if (!day_str) break;                                    // geen day -> stop met parsen
        int day_num = parse_int(day_str);                       // parse day number

        const char *tasks_str = strstr(*ptr, "\"tasks\": [");   // kijken voor taken ("tasks": [)
        Task *tasks = NULL;                                     // initialiseer taken lijst
        if (tasks_str) {                                        // als er taken zijn
            const char *tptr = tasks_str + 9;                   // verplaats pointer naar na "tasks": [
            tasks = task_parse(&tptr);                          // parse taken lijst
        }

        Day *d = malloc(sizeof(Day));   // maak nieuwe dag aan
        d->day = day_num;               // zet dag nummer
        d->tasks = tasks;               // zet taken lijst
        d->left = d->right = NULL;      // initialiseer kinderen

        
        if (!root) root = d;                                    // als geen root root op nieuwe dag
        else {                                                  // anders, voeg de dag toe aan de BST
            Day *cur = root;                                    // begin bij de root
            while (1) {                                         // loop
                if (day_num < cur->day) {                       // als dag nummer kleiner - ga links
                    if (!cur->left) { cur->left = d; break; }   // geen linker kind - voeg toe
                    cur = cur->left;                            // naar linker kind
                } else {                                        // als dag nummer groter of gelijk - ga rechts
                    if (!cur->right) { cur->right = d; break; } // geen rechter kind - voeg toe
                    cur = cur->right;                           // naar rechter kind
                }
            }
        }

        const char *next = strchr(*ptr, '}');       // volgende dag zoeken
        if (!next) break;                           // geen volgende dag -> stop met parsen
        *ptr = next + 1;                            // verplaats pointer naar na de volgende dag
    }
    return root; // geef root van de BST door
}


Month* month_parse(const char** ptr)                            // Helper om maanden array te parsen
{
    Month *root = NULL;                                         // Root van de BST
    while (**ptr && **ptr != ']') {                             // Zolang in de array
        const char *m_str = strstr(*ptr, "\"month\":");         // kijken voor "month": n
        if (!m_str) break;                                      // geen month -> stop met parsen
        int month_num = parse_int(m_str);                       // parse month nummer

        const char *days_str = strstr(*ptr, "\"days\": [");     // kijken voor dagen ("days": [)
        Day *days = NULL;                                       // initialiseer dagen BST
        if (days_str) {                                         // als er dagen zijn
            const char *dptr = days_str + 7;                    // verplaats pointer naar na "days": [
            days = day_parse(&dptr);                            // parse dagen BST
        }   

        Month *m = malloc(sizeof(Month));    // maak nieuwe maand aan
        m->month = month_num;                // zet maand nummer
        m->days = days;                      // zet dagen BST
        m->left = m->right = NULL;           // initialiseer kinderen

        if (!root) root = m;                                        // als geen root root op nieuwe maand
        else {                                                      // wel root 
            Month *cur = root;                                      // begin bij de root
            while (1) {                                             // loop
                if (month_num < cur->month) {                       // als maand nummer kleiner
                    if (!cur->left) { cur->left = m; break; }       // geen linker kind - voeg toe
                    cur = cur->left;                                // naar linker kind
                } else {                                            // als maand nummer groter of gelijk
                    if (!cur->right) { cur->right = m; break; }     // geen rechter kind - voeg toe
                    cur = cur->right;                               // naar rechter kind
                }
            }
        }

        const char *next = strchr(*ptr, '}');   // volgende maand zoeken
        if (!next) break;                       // geen volgende maand -> stop met parsen
        *ptr = next + 1;                        // verplaats pointer naar na de volgende maand
    }   
    return root; // geef root van de BST door
}

Year* year_parse(const char** ptr)                                  // Helper om jaren array te parsen
{
    Year *root = NULL;                                              // Root van de BST
    while (**ptr && **ptr != ']') {                                 // Zolang in de array
        const char *y_str = strstr(*ptr, "\"year\":");              // kijken voor "year": n
        if (!y_str) break;                                          // geen year -> stop met parsen
        int year_num = parse_int(y_str);                            // parse year nummer

        const char *months_str = strstr(*ptr, "\"months\": [");     // kijken voor maanden ("months": [)
        Month *months = NULL;                                       // initialiseer maanden BST
        if (months_str) {                                           // als er maanden zijn
            const char *mptr = months_str + 9;                      // verplaats pointer naar na "months": [
            months = month_parse(&mptr);                            // parse maanden BST
        }

        Year *y = malloc(sizeof(Year)); // maak nieuwe jaar aan
        y->year = year_num;             // zet jaar nummer
        y->months = months;             // zet maanden BST
        y->left = y->right = NULL;      // initialiseer kinderen

        if (!root) root = y;                                        // als geen root root op nieuwe jaar
        else {                                                      // wel root
            Year *cur = root;                                       // begin bij de root
            while (1) {                                             // loop
                if (year_num < cur->year) {                         // als jaar nummer kleiner
                    if (!cur->left) { cur->left = y; break; }       // geen linker kind - voeg toe
                    cur = cur->left;                                // naar linker kind
                } else {                                            // als jaar nummer groter of gelijk
                    if (!cur->right) { cur->right = y; break; }     // geen rechter kind - voeg toe
                    cur = cur->right;                               // naar rechter kind
                }
            }
        }

        const char *next = strchr(*ptr, '}'); // volgende jaar zoeken
        if (!next) break;                     // geen volgende jaar -> stop met parsen
        *ptr = next + 1;                      // verplaats pointer naar na de volgende jaar
    }
    return root; // geef root van de BST door
}