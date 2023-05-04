#include "include/utils.h"

typedef struct Recycle {
    void* var;
    void* next;
} Recycle;

Recycle* tail = NULL;

void add_recycle(void* v) {
    Recycle* r = (Recycle*)malloc(sizeof(Recycle));

    r->var  = v;
    r->next = tail;
    tail    = r;
}

void free_recycle() {
    Recycle* next = NULL;

    while (tail) {
        next = tail->next;
        free(tail->var);
        free(tail);
        tail = next;
    }
}