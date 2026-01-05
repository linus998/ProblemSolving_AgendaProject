
// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: contains.c                                                                         //
// Description: Implementatie van de contains functie voor print met tekst search               //
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
