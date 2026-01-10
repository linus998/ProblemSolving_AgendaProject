
// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: helpers.c                                                                          //
// Description: Implementatie van de helper functies voor agenda.c                              //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================

// C standard library
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// ===========================================================================================
// Function Implementations
// ===========================================================================================

int contains_ci(const char* hay, const char* needle)        // needle in haystack zoeken, case insensitive
{
    if (!hay || !needle) return 0;              // foutafhandeling null pointers
    size_t n = strlen(needle);                  // lengte van needle
    if (n == 0) return 1;                       // lege needle altijd gevonden
    for (const char* p = hay; *p; ++p) {        // loop door haystack
        size_t i = 0;                           // index voor needle
        for (; i < n && p[i]; ++i) {            // loop door needle
            if (tolower((unsigned char)p[i]) != tolower((unsigned char)needle[i])) break; // vergelijk karakters case insensitive
        }
        if (i == n) return 1; // als hele needle is gevonden, return 1
    }
    return 0; // needle niet gevonden, return 0
}

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

static void trim_whitespace(char *s)
{
    if (!s) return;// check dat er een karakter is    
    // trim leaing whitespaces from char array             
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);
    // trim trailing whitespaces from char array
    char *end = s + strlen(s) - 1;
    while (end >= s && isspace((unsigned char)*end)) { *end = '\0'; end--; }
}