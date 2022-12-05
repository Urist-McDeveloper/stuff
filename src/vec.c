#include "vec/vec.h"

#include <stdlib.h>
#include <string.h>
#ifdef DEBUG
#include <stdio.h>
#endif

#define MIN_CAP 4

int VecEnsureCap(Vec* v, uint32_t c, size_t s) {
    if (c <= v->cap) {
#ifdef DEBUG
        printf("[i] VecEnsureCap(%p, %u, %lu): sufficient capacity (%u)\n", v, c, s, v->cap);
#endif
        return 0;
    }

    uint32_t cap = v->cap;
    if (cap < MIN_CAP) cap = MIN_CAP;

    while (cap < c)
        cap *= 2;

    size_t size = cap * s;
    void*  arr = realloc(v->arr, size);

    if (arr) {
#ifdef DEBUG
        printf("[i] VecEnsureCap(%p, %u, %lu): realloc(%p, %lu) == %p\n", v, c, s, v->arr, size,
               arr);
#endif
        v->cap = cap;
        v->arr = arr;
        return 0;
    } else {
#ifdef DEBUG
        printf("[e] VecEnsureCap(%p, %u, %lu): realloc(%p, %lu) failed\n", v, c, s, v->arr, size);
#endif
        return 1;
    }
}

void* VecInsPtr(Vec* v, uint32_t i, size_t s) {
    if (i < v->len) {
        if (VecEnsureFit(v, 1, s)) return NULL;

        char*  src = (char*)v->arr + s * i;
        char*  dst = src + s;
        size_t n = s * (v->len - i);

        memmove(dst, src, n);
        v->len += 1;
    } else {
        if (VecEnsureCap(v, i + 1, s)) return NULL;
        v->len = i + 1;
    }
    return (char*)v->arr + s * i;
}

int VecDel(Vec* v, uint32_t i, size_t s) {
    if (i >= v->len) return 1;
    if (i < (--v->len)) {
        char*  dst = (char*)v->arr + s * i;
        char*  src = dst + s;
        size_t n = s * (v->len - i);
        memmove(dst, src, n);
    }
    return 0;
}
