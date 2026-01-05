#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Structs/day.h"
#include "Structs/task.h"
#include "Structs/month.h"
#include "Structs/year.h"

char* read_file(const char *filename);
void parse_string(const char* src, char* dest, int max_len);
int parse_int(const char *src);
Task* task_parse(const char** ptr);
Day* day_parse(const char** ptr);
Month* month_parse(const char** ptr);
Year* year_parse(const char** ptr);