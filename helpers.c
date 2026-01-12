
// =============================================================================================//
// Name: Linus Beheydt                                                                          //
// Nubmer: 2508401                                                                              //
// Filename: helpers.c                                                                          //
// Description: Implementatie van de helper functies voor agenda.c                              //
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

void trim_whitespace(char *s)
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

// ===========================================================================================
// Input helpers (validated prompts)
// ===========================================================================================

int prompt_int(const char *prompt, const char *format_hint)
{
    char buf[128];
    int value;
    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) { clearerr(stdin); continue; }
        // allow leading/trailing whitespace
        trim_whitespace(buf);
        if (buf[0] == '\0') {
            printf("Invalid input. Format: %s\n", format_hint ? format_hint : "integer (e.g., 1)");
            continue;
        }
        if (sscanf(buf, "%d", &value) == 1) return value;
        printf("Invalid input. Format: %s\n", format_hint ? format_hint : "integer (e.g., 1)");
    }
}

void prompt_two_ints(const char *prompt, int *a, int *b, const char *format_hint)
{
    char buf[128];
    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) { clearerr(stdin); continue; }
        trim_whitespace(buf);
        if ((sscanf(buf, "%d %d", a, b) != 2) || (*a > 23 || *b > 59)){       
            printf("Invalid input. Format: %s\n", format_hint ? format_hint : "two integers separated by space (e.g., 9 30)");
        } else {
            return;
        }
    }
}

void prompt_three_ints(const char *prompt, int *a, int *b, int *c, const char *format_hint)
{
    char buf[256];
    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) { clearerr(stdin); continue; }
        trim_whitespace(buf);
        if (sscanf(buf, "%d %d %d", a, b, c) == 3) return;
        printf("Invalid input. Format: %s\n", format_hint ? format_hint : "three integers separated by space (e.g., 2026 1 31)");
    }
}

void prompt_string(const char *prompt, char *buf, size_t bufsize, const char *format_hint)
{
    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(buf, bufsize, stdin)) { clearerr(stdin); continue; }
        // remove trailing newline and trim
        buf[strcspn(buf, "\n")] = '\0';
        trim_whitespace(buf);
        if (buf[0] == '\0') {
            printf("Invalid input. Format: %s\n", format_hint ? format_hint : "non-empty text");
            continue;
        }
        return;
    }
}
