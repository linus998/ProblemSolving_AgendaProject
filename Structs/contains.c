
// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: contains.c                                                                         //
// Description: Implementation file for the Day struct and related functions.                   //
// =============================================================================================//

// ===========================================================================================
// Includes
// ===========================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// ===========================================================================================
// Function Implementations
// ===========================================================================================

int contains_ci(const char* hay, const char* needle) {
    if (!hay || !needle) return 0;
    size_t n = strlen(needle);
    if (n == 0) return 1;
    for (const char* p = hay; *p; ++p) {
        size_t i = 0;
        for (; i < n && p[i]; ++i) {
            if (tolower((unsigned char)p[i]) != tolower((unsigned char)needle[i])) break;
        }
        if (i == n) return 1;
    }
    return 0;
}
