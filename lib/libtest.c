#include "libtest.h"

#include "cstring.h"
#include <stdio.h>

#define TT_LINE1 "============================================================\n"
#define TT_LINE2 "------------------------------------------------------------\n"
#define TT_COLOR_RED "[1;31m"
#define TT_COLOR_GREEN "[1;32m"

const char *_tt_current_expression = NULL;
const char *_tt_current_file = NULL;
int _tt_current_line = 0;
int _tt_current_result = 0;

int _tt_passed = 0;
int _tt_failed = 0;

void tt_execute(const char *name, void (*test_function)())
{
    if (_tt_current_result)
        return;

    printf(TT_LINE1);
    printf("         > %s\n", name);
    printf(TT_LINE2);

    _tt_current_result = 1;

    test_function();

    if (_tt_current_result == 0)
    {
        _tt_passed++;
    }
    else
    {
        _tt_failed++;
    }
}

int tt_assert(const char *expression, const char *file, int line, int result)
{
    _tt_current_expression = expression;
    _tt_current_file = file;
    _tt_current_line = line;
    _tt_current_result = result;

    if (result == 0)
    {
        printf("\x1b%s[passed]\x1b[0m | (%s)\n",
               TT_COLOR_GREEN,
               _tt_current_expression);
    }
    else
    {
        printf("\x1b%s[failed]\x1b[0m | (%s)\n",
               TT_COLOR_RED,
               _tt_current_expression);

        printf("         | %s:%d\n",
               _tt_current_file,
               _tt_current_line);
    }

    return result;
}

int tt_report(void)
{
    printf(TT_LINE1);

    if (_tt_failed)
    {
        printf("         | passed:%d, failed:%d, total:%d\n",
               _tt_passed, _tt_failed, (_tt_passed + _tt_failed));

        return -1;
    }
    else
    {
        printf("         | total:%d\n", _tt_passed);
        return 0;
    }

    printf(TT_LINE2);
}


