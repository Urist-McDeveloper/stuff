#include "vec/str.h"
#include "greatest.h"

static char* const strs[] = {
    "some", "body", "once", "told", "me", "the", "world", "is", "gonna", "roll", "me",
};
static const int strs_len = sizeof(strs) / sizeof(strs[0]);

TEST cap() {
    StrVec v = {0};
    StrVecEnsureCap(&v, 10);

    GREATEST_ASSERT_GTE(v.cap, 10);

    free(v.arr);
    PASS();
}

TEST fit() {
    StrVec v = {0};
    StrVecEnsureFit(&v, 5);

    GREATEST_ASSERT_GTE(v.cap, 5);

    free(v.arr);
    PASS();
}

TEST push() {
    StrVec    v = {0};
    const int count = 3210;

    for (int i = 0; i < count; i++)
        StrVecPush(&v, strs[i % strs_len]);

    GREATEST_ASSERT_GTE(v.cap, (unsigned int)count);
    GREATEST_ASSERT_EQ((unsigned int)count, v.len);

    for (int i = 0; i < count; i++)
        GREATEST_ASSERT_STR_EQ(strs[i % strs_len], v.arr[i]);

    free(v.arr);
    PASS();
}

TEST push_ptr() {
    StrVec    v = {0};
    const int count = 3210;

    for (int i = 0; i < count; i++)
        *StrVecPushPtr(&v) = strs[i % strs_len];

    GREATEST_ASSERT_GTE(v.cap, (unsigned int)count);
    GREATEST_ASSERT_EQ((unsigned int)count, v.len);

    for (int i = 0; i < count; i++)
        GREATEST_ASSERT_STR_EQ(strs[i % strs_len], v.arr[i]);

    free(v.arr);
    PASS();
}

TEST pop_empty() {
    StrVec v = {0};

    GREATEST_ASSERT_EQ(NULL, StrVecPop(&v));

    free(v.arr);
    PASS();
}

TEST pop() {
    StrVec v = {0};
    StrVecPush(&v, "Never Gonna Give You Up");

    char** popped = StrVecPop(&v);

    GREATEST_ASSERT_EQ(0, v.len);
    GREATEST_ASSERT_EQ(v.arr, popped);
    GREATEST_ASSERT_STR_EQ(v.arr[0], *popped);

    free(v.arr);
    PASS();
}

TEST ins_ptr() {
    StrVec v = {0};
    for (int i = 0; i < 3; i++)
        StrVecPush(&v, strs[i]);

    char** ins = StrVecInsPtr(&v, 1);

    GREATEST_ASSERT_EQ(4, v.len);
    GREATEST_ASSERT_EQ(v.arr + 1, ins);
    GREATEST_ASSERT_STR_EQ(strs[0], v.arr[0]);
    GREATEST_ASSERT_STR_EQ(strs[1], v.arr[2]);
    GREATEST_ASSERT_STR_EQ(strs[2], v.arr[3]);

    free(v.arr);
    PASS();
}

TEST ins_ptr_bounds() {
    StrVec v = {0};

    char** ins = StrVecInsPtr(&v, 5);

    GREATEST_ASSERT_EQ(6, v.len);
    GREATEST_ASSERT_EQ(v.arr + 5, ins);

    free(v.arr);
    PASS();
}

TEST ins() {
    StrVec v = {0};
    for (int i = 0; i < 3; i++)
        StrVecPush(&v, strs[i]);

    StrVecIns(&v, 1, "Eighteen naked cowboys in the showers at Ram Ranch");

    GREATEST_ASSERT_EQ(4, v.len);
    GREATEST_ASSERT_STR_EQ(strs[0], v.arr[0]);
    GREATEST_ASSERT_STR_EQ("Eighteen naked cowboys in the showers at Ram Ranch", v.arr[1]);
    GREATEST_ASSERT_STR_EQ(strs[1], v.arr[2]);
    GREATEST_ASSERT_STR_EQ(strs[2], v.arr[3]);

    free(v.arr);
    PASS();
}

TEST ins_bounds() {
    StrVec v = {0};

    StrVecIns(&v, 5, "Gotye - Some Mother That I Used To Do");

    GREATEST_ASSERT_EQ(6, v.len);
    GREATEST_ASSERT_STR_EQ("Gotye - Some Mother That I Used To Do", v.arr[5]);

    free(v.arr);
    PASS();
}

TEST del() {
    StrVec v = {0};
    for (int i = 0; i < 3; i++)
        StrVecPush(&v, strs[i]);

    GREATEST_ASSERT_FALSE(StrVecDel(&v, 1));

    GREATEST_ASSERT_EQ(2, v.len);
    GREATEST_ASSERT_STR_EQ(strs[0], v.arr[0]);
    GREATEST_ASSERT_STR_EQ(strs[2], v.arr[1]);

    free(v.arr);
    PASS();
}

TEST del_bounds() {
    StrVec v = {0};

    GREATEST_ASSERT(StrVecDel(&v, 2));

    free(v.arr);
    PASS();
}

TEST del_swap() {
    StrVec v = {0};
    for (int i = 0; i < 3; i++)
        StrVecPush(&v, strs[i]);

    GREATEST_ASSERT_FALSE(StrVecDelSwap(&v, 0));

    GREATEST_ASSERT_EQ(2, v.len);
    GREATEST_ASSERT_STR_EQ(strs[2], v.arr[0]);
    GREATEST_ASSERT_STR_EQ(strs[1], v.arr[1]);

    free(v.arr);
    PASS();
}

TEST del_swap_bounds() {
    StrVec v = {0};

    GREATEST_ASSERT(StrVecDelSwap(&v, 2));

    free(v.arr);
    PASS();
}

SUITE(StrVecSuite) {

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
