#ifndef VEC_H
#define VEC_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    /* Capacity of {arr}. Changing this manually is a bad idea. */
    uint32_t cap;
    /* Number of stored elements in {arr}. Decreasing this manually is ok. */
    uint32_t len;
    /* The allocated array. Must be freed by struct owner. */
    void* arr;
} Vec;

/*
 * Ensures that {v} can fit {c} elements of size {s}.
 * Returns 1 if {v->arr} could not be reallocd.
 */
int VecEnsureCap(Vec* v, uint32_t c, size_t s);

/*
 * Inserts empty element of size {s} into {v} at index {i}.
 * If {i} < {v->len}, shifts every element with index j >= {i} one position to the right.
 *
 * Returns NULL if {v->arr} could not be reallocd.
 */
void* VecInsPtr(Vec* v, uint32_t i, size_t s);

/*
 * Removes element of size {s} at index {i} from {v}.
 * Decreases {v->len} and shifts every element with index j > {i} one position to the left.
 *
 * Returns 1 if {i} >= {v->arr.len}.
 */
int VecDel(Vec* v, uint32_t i, size_t s);

/*
 * Ensures that {v} can fit {c} more elements of size {s}.
 * Returns 1 if {v->arr} could not be reallocd.
 */
static inline int VecEnsureFit(Vec* v, uint32_t c, size_t s) {
    return VecEnsureCap(v, v->len + c, s);
}

/*
 * Template macro to create typed `Vec` structs.
 * `DeclareTypedVec(char*, Str)` will create `StrVec` with `char*` elements.
 */
#define DeclareTypedVec(Type, Pfx)                                                                 \
    typedef struct {                                                                               \
        uint32_t cap;                                                                              \
        uint32_t len;                                                                              \
        Type*    arr;                                                                              \
    } Pfx##Vec;                                                                                    \
                                                                                                   \
    static inline int Pfx##VecEnsureCap(Pfx##Vec* v, uint32_t c) {                                 \
        return VecEnsureCap((Vec*)v, c, sizeof(Type));                                             \
    }                                                                                              \
                                                                                                   \
    static inline int Pfx##VecEnsureFit(Pfx##Vec* v, uint32_t c) {                                 \
        return VecEnsureFit((Vec*)v, c, sizeof(Type));                                             \
    }                                                                                              \
                                                                                                   \
    static inline int Pfx##VecPush(Pfx##Vec* v, Type t) {                                          \
        int e = Pfx##VecEnsureFit(v, 1);                                                           \
        if (e == 0) v->arr[v->len++] = t;                                                          \
        return e;                                                                                  \
    }                                                                                              \
                                                                                                   \
    static inline Type* Pfx##VecPushPtr(Pfx##Vec* v) {                                             \
        int e = Pfx##VecEnsureFit(v, 1);                                                           \
        return e == 0 ? v->arr + (v->len++) : NULL;                                                \
    }                                                                                              \
                                                                                                   \
    static inline Type* Pfx##VecPop(Pfx##Vec* v) {                                                 \
        return v->len == 0 ? NULL : v->arr + (--v->len);                                           \
    }                                                                                              \
                                                                                                   \
    static inline Type* Pfx##VecInsPtr(Pfx##Vec* v, uint32_t i) {                                  \
        return VecInsPtr((Vec*)v, i, sizeof(Type));                                                \
    }                                                                                              \
                                                                                                   \
    static inline int Pfx##VecIns(Pfx##Vec* v, uint32_t i, Type t) {                               \
        Type* e = Pfx##VecInsPtr(v, i);                                                            \
        if (e != NULL) *e = t;                                                                     \
        return e == NULL;                                                                          \
    }                                                                                              \
                                                                                                   \
    static inline int Pfx##VecDel(Pfx##Vec* v, uint32_t i) {                                       \
        return VecDel((Vec*)v, i, sizeof(Type));                                                   \
    }                                                                                              \
                                                                                                   \
    static inline int Pfx##VecDelSwap(Pfx##Vec* v, uint32_t i) {                                   \
        if (i >= v->len) return 1;                                                                 \
        if (i < (--v->len)) {                                                                      \
            v->arr[i] = v->arr[v->len];                                                            \
        }                                                                                          \
        return 0;                                                                                  \
    }

/* Use this if, for some reason, you need untyped vector. */
DeclareTypedVec(void*, Void);

#endif
