#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Structs/day.h"
#include "Structs/task.h"
#include "Structs/month.h"
#include "Structs/year.h"

// --- Helper to read entire file ---
char* read_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    if (!buffer) { fclose(fp); return NULL; }

    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);
    return buffer;
}

// --- Helper to extract a string value between quotes ---
void parse_string(const char* src, char* dest, int max_len) {
    const char *start = strchr(src, '"');
    if (!start) { dest[0] = '\0'; return; }
    start++;
    const char *end = strchr(start, '"');
    if (!end) { dest[0] = '\0'; return; }
    int len = end - start;
    if (len >= max_len) len = max_len - 1;
    strncpy(dest, start, len);
    dest[len] = '\0';
}

// --- Helper to extract integer after colon ---
int parse_int(const char *src) {
    const char *colon = strchr(src, ':');
    if (!colon) return 0;
    return strtol(colon + 1, NULL, 10);
}

// --- Parse tasks array ---
Task* task_parse(const char** ptr) {
    Task *head = NULL;
    while (**ptr && **ptr != ']') {
        // Look for "id": n
        const char *id_str = strstr(*ptr, "\"id\":");
        if (!id_str) break;
        int id = parse_int(id_str);

        // start and end time
        const char *start_str = strstr(*ptr, "\"start\":");
        int sh = 0, sm = 0, eh = 0, em = 0;
        char s[6] = {0}, e[6] = {0};
        if (start_str) {
            parse_string(start_str, s, sizeof(s));
            sscanf(s, "%d:%d", &sh, &sm);
        }
        const char *end_str = strstr(*ptr, "\"end\":");
        if (end_str) {
            parse_string(end_str, e, sizeof(e));
            sscanf(e, "%d:%d", &eh, &em);
        }

        // title, description, location
        char title[128] = "", desc[256] = "", loc[128] = "";
        const char *t_str = strstr(*ptr, "\"title\":");
        if (t_str) parse_string(t_str, title, sizeof(title));
        const char *d_str = strstr(*ptr, "\"description\":");
        if (d_str) parse_string(d_str, desc, sizeof(desc));
        const char *l_str = strstr(*ptr, "\"location\":");
        if (l_str) parse_string(l_str, loc, sizeof(loc));

        Task *t = task_create(id, sh, sm, eh, em, title, desc, loc);
        task_add(&head, t);

        const char *next = strchr(*ptr, '}');  // move to next task
        if (!next) break;
        *ptr = next + 1;
    }
    return head;
}

// --- Parse days array ---
Day* day_parse(const char** ptr) {
    Day *root = NULL;
    while (**ptr && **ptr != ']') {
        const char *day_str = strstr(*ptr, "\"day\":");
        if (!day_str) break;
        int day_num = parse_int(day_str);

        const char *tasks_str = strstr(*ptr, "\"tasks\": [");
        Task *tasks = NULL;
        if (tasks_str) {
            const char *tptr = tasks_str + 9;
            tasks = task_parse(&tptr);
        }

        Day *d = malloc(sizeof(Day));
        d->day = day_num;
        d->tasks = tasks;
        d->left = d->right = NULL;

        // Simple BST insert by day
        if (!root) root = d;
        else {
            Day *cur = root;
            while (1) {
                if (day_num < cur->day) {
                    if (!cur->left) { cur->left = d; break; }
                    cur = cur->left;
                } else {
                    if (!cur->right) { cur->right = d; break; }
                    cur = cur->right;
                }
            }
        }

        const char *next = strchr(*ptr, '}');
        if (!next) break;
        *ptr = next + 1;
    }
    return root;
}

// --- Parse months array ---
Month* month_parse(const char** ptr) {
    Month *root = NULL;
    while (**ptr && **ptr != ']') {
        const char *m_str = strstr(*ptr, "\"month\":");
        if (!m_str) break;
        int month_num = parse_int(m_str);

        const char *days_str = strstr(*ptr, "\"days\": [");
        Day *days = NULL;
        if (days_str) {
            const char *dptr = days_str + 7;
            days = day_parse(&dptr);
        }

        Month *m = malloc(sizeof(Month));
        m->month = month_num;
        m->days = days;
        m->left = m->right = NULL;

        if (!root) root = m;
        else {
            Month *cur = root;
            while (1) {
                if (month_num < cur->month) {
                    if (!cur->left) { cur->left = m; break; }
                    cur = cur->left;
                } else {
                    if (!cur->right) { cur->right = m; break; }
                    cur = cur->right;
                }
            }
        }

        const char *next = strchr(*ptr, '}');
        if (!next) break;
        *ptr = next + 1;
    }
    return root;
}

// --- Parse years array ---
Year* year_parse(const char** ptr) {
    Year *root = NULL;
    while (**ptr && **ptr != ']') {
        const char *y_str = strstr(*ptr, "\"year\":");
        if (!y_str) break;
        int year_num = parse_int(y_str);

        const char *months_str = strstr(*ptr, "\"months\": [");
        Month *months = NULL;
        if (months_str) {
            const char *mptr = months_str + 9;
            months = month_parse(&mptr);
        }

        Year *y = malloc(sizeof(Year));
        y->year = year_num;
        y->months = months;
        y->left = y->right = NULL;

        if (!root) root = y;
        else {
            Year *cur = root;
            while (1) {
                if (year_num < cur->year) {
                    if (!cur->left) { cur->left = y; break; }
                    cur = cur->left;
                } else {
                    if (!cur->right) { cur->right = y; break; }
                    cur = cur->right;
                }
            }
        }

        const char *next = strchr(*ptr, '}');
        if (!next) break;
        *ptr = next + 1;
    }
    return root;
}