// ===================================-- H O O F D I N G --=====================================//
// Name: Linus Beheydt                                                                          //
// Number: 2508401                                                                              //
// Filename: import.h                                                                           //
// Description: import en export functie prototypes                                             //
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
// Functie Prototypes
// ===========================================================================================

char* read_file(const char *filename);
void parse_string(const char* src, char* dest, int max_len);
int parse_int(const char *src);
Task* task_parse(const char** ptr);
Day* day_parse(const char** ptr);
Month* month_parse(const char** ptr);
Year* year_parse(const char** ptr);