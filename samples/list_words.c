#include <cfile.h>

#include <ctype.h>
#include <print.h>

bool get_word(char **start, CString *result)
{
    // end of buffer ?
    if (**start == '\0')
        return false;

    // start of line.
    char *first = *start;
    char *p = first;

    while (1)
    {
        if (isalpha(*p) || *p =='_')
        {
            char *word = p;
            while (isalnum(*p) || *p =='_')
                ++p;

            cstr_clear(result);
            cstr_append_len(result, word, p - word);

            // move to next word.
            *start = p;

            return true;
        }
        else if (*p == '\0')
        {
            return false;
        }

        ++p;
    }
}

int main()
{
    const char *filepath = "/usr/local/include/tinyc/print.h";

    cfile_auto CFile *file = cfile_new();

    if (!cfile_read(file, filepath))
        return 1;

    cstr_auto CString *line = cstr_new_size(32);
    cstr_auto CString *word = cstr_new_size(32);

    while (cfile_getline(file, line))
    {
        char *curr = cstr_data(line);

        while (get_word(&curr, word))
        {
            print(c_str(word));
        }
    }

    return 0;
}


