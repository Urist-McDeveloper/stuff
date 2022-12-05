#include <stdio.h>

#include "vec/str.h"

int main() {
    StrVec v = {0};
    StrVecPush(&v, "Hello, world!");
    printf("%s\n", *StrVecPop(&v));
}
