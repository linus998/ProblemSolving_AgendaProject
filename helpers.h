// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: contains.h                                                                         //
// Description: Header file for the Day struct and related functions.                           //
// =============================================================================================//

// ===========================================================================================
// Header Guards
// ===========================================================================================
#ifndef HELPERS_H               
#define HELPERS_H

#include <stddef.h>

// ===========================================================================================
// Function Prototypes
// ===========================================================================================

int contains_ci(const char* hay, const char* needle);
char* read_file(const char *filename);
void trim_whitespace(char *s);

// Input helper functions: prompt and validate user input
int prompt_int(const char *prompt, const char *format_hint);
void prompt_two_ints(const char *prompt, int *a, int *b, const char *format_hint);
void prompt_three_ints(const char *prompt, int *a, int *b, int *c, const char *format_hint);
void prompt_string(const char *prompt, char *buf, size_t bufsize, const char *format_hint);

// ===========================================================================================
// end Header Guards
// ===========================================================================================
#endif