#include "vec/int.h"
#include "greatest.h"

TEST cap() {
    IntVec v = {0};
    IntVecEnsureCap(&v, 10);

    GREATEST_ASSERT_GTE(v.cap, 10);

    free(v.arr);
    PASS();
}

TEST fit() {
    IntVec v = {0};
    IntVecEnsureFit(&v, 5);

    GREATEST_ASSERT_GTE(v.cap, 5);

    free(v.arr);
    PASS();
}

TEST push() {
    IntVec    v = {0};
    const int count = 3210;

    for (int i = 0; i < count; i++)
        IntVecPush(&v, i);

    GREATEST_ASSERT_GTE(v.cap, (unsigned int)count);
    GREATEST_ASSERT_EQ((unsigned int)count, v.len);

    for (int i = 0; i < count; i++)
        GREATEST_ASSERT_EQ(i, v.arr[i]);

    free(v.arr);
    PASS();
}

TEST push_ptr() {
    IntVec    v = {0};
    const int count = 3210;

    for (int i = 0; i < count; i++)
        *IntVecPushPtr(&v) = i;

    GREATEST_ASSERT_GTE(v.cap, (unsigned int)count);
    GREATEST_ASSERT_EQ((unsigned int)count, v.len);

    for (int i = 0; i < count; i++)
        GREATEST_ASSERT_EQ(i, v.arr[i]);

    free(v.arr);
    PASS();
}

TEST pop_empty() {
    IntVec v = {0};

    GREATEST_ASSERT_EQ(NULL, IntVecPop(&v));

    free(v.arr);
    PASS();
}

TEST pop() {
    IntVec v = {0};
    IntVecPush(&v, 123456789);

    int* popped = IntVecPop(&v);

    GREATEST_ASSERT_EQ(0, v.len);
    GREATEST_ASSERT_EQ(v.arr, popped);
    GREATEST_ASSERT_EQ(v.arr[0], *popped);

    free(v.arr);
    PASS();
}

TEST ins_ptr() {
    IntVec v = {0};
    for (int i = 0; i < 3; i++)
        IntVecPush(&v, i);

    int* ins = IntVecInsPtr(&v, 1);

    GREATEST_ASSERT_EQ(4, v.len);
    GREATEST_ASSERT_EQ(v.arr + 1, ins);
    GREATEST_ASSERT_EQ(0, v.arr[0]);
    GREATEST_ASSERT_EQ(1, v.arr[2]);
    GREATEST_ASSERT_EQ(2, v.arr[3]);

    free(v.arr);
    PASS();
}

TEST ins_ptr_bounds() {
    IntVec v = {0};

    int* ins = IntVecInsPtr(&v, 5);

    GREATEST_ASSERT_EQ(6, v.len);
    GREATEST_ASSERT_EQ(v.arr + 5, ins);

    free(v.arr);
    PASS();
}

TEST ins() {
    IntVec v = {0};
    for (int i = 0; i < 3; i++)
        IntVecPush(&v, i);

    IntVecIns(&v, 1, 5);

    GREATEST_ASSERT_EQ(4, v.len);
    GREATEST_ASSERT_EQ(0, v.arr[0]);
    GREATEST_ASSERT_EQ(5, v.arr[1]);
    GREATEST_ASSERT_EQ(1, v.arr[2]);
    GREATEST_ASSERT_EQ(2, v.arr[3]);

    free(v.arr);
    PASS();
}

TEST ins_bounds() {
    IntVec v = {0};

    IntVecIns(&v, 5, 321);

    GREATEST_ASSERT_EQ(6, v.len);
    GREATEST_ASSERT_EQ(321, v.arr[5]);

    free(v.arr);
    PASS();
}

TEST del() {
    IntVec v = {0};
    for (int i = 0; i < 3; i++)
        IntVecPush(&v, i);

    GREATEST_ASSERT_FALSE(IntVecDel(&v, 1));

    GREATEST_ASSERT_EQ(2, v.len);
    GREATEST_ASSERT_EQ(0, v.arr[0]);
    GREATEST_ASSERT_EQ(2, v.arr[1]);

    free(v.arr);
    PASS();
}

TEST del_bounds() {
    IntVec v = {0};

    GREATEST_ASSERT(IntVecDel(&v, 2));

    free(v.arr);
    PASS();
}

TEST del_swap() {
    IntVec v = {0};
    for (int i = 0; i < 3; i++)
        IntVecPush(&v, i);

    GREATEST_ASSERT_FALSE(IntVecDelSwap(&v, 0));

    GREATEST_ASSERT_EQ(2, v.len);
    GREATEST_ASSERT_EQ(2, v.arr[0]);
    GREATEST_ASSERT_EQ(1, v.arr[1]);

    free(v.arr);
    PASS();
}

TEST del_swap_bounds() {
    IntVec v = {0};

    GREATEST_ASSERT(IntVecDelSwap(&v, 2));

    free(v.arr);
    PASS();
}

SUITE(IntVecSuite) {

    RUN_TEST(cap);
    RUN_TEST(fit);

    RUN_TEST(push);
    RUN_TEST(push_ptr);

    RUN_TEST(pop_empty);
    RUN_TEST(pop);

    RUN_TEST(ins_ptr);
    RUN_TEST(ins_ptr_bounds);

    RUN_TEST(ins);
    RUN_TEST(ins_bounds);

    RUN_TEST(del);
    RUN_TEST(del_bounds);

    RUN_TEST(del_swap);
    RUN_TEST(del_swap_bounds);
}
