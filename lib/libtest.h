#ifndef LIBTINYTEST_H
#define LIBTINYTEST_H

#define ASSERT(expression) \
    if (tt_assert((#expression), \
                  __FILE__, __LINE__, (expression) ? 0 : 1)) \
        return

#if 0
#define ASSERT_EQ(expected, actual) \
            ASSERT((#actual), (expected) == (actual))

#define ASSERT_STREQ(expected, actual) \
            ASSERT((#actual), strcmp((expected), (actual)) == 0)
#endif

#define RUN(test_function) \
    tt_execute((#test_function), (test_function))

#define TEST_REPORT() tt_report()

int tt_assert(const char *expression,
              const char *file, int line, int result);
void tt_execute(const char *name, void (*test_function)());
int tt_report(void);

#endif // LIBTINYTEST_H


