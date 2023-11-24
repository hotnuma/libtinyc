#include "libtest.h"
#include "cfile.h"

#define _TESTFILE "/tmp/tinyc_cfile.txt"

void test_cfile()
{
    ASSERT(dir_exists("/tmp"));

    file_remove(_TESTFILE);

    CFile *file = cfile_new();
    cfile_open(file, _TESTFILE, "wb");

    cfile_write(file, "bla");
    cfile_write(file, "\n");
    cfile_write(file, "ble");
    cfile_write(file, "\n");
    cfile_write(file, "blie");
    cfile_write(file, "\n");

    cfile_flush(file);
    cfile_close(file);

    cfile_read(file, _TESTFILE);
    ASSERT(cstr_compare(cfile_buffer(file), "bla\nble\nblie\n", true) == 0);

    file_remove(_TESTFILE);

    cfile_free(file);
}

#if 0
#define CFileAuto GC_CLEANUP(_freeCFile) CFile
bool cfile_getline(CFile *cfile, CString *result);
void cfile_writefmt(CFile *cfile, const char *fmt, ...);
char* cfile_data(CFile *cfile);
int cfile_size(CFile *cfile);
FILE* cfile_fp(CFile *cfile);
int cfile_fd(CFile *cfile);
bool file_exists(const char *fileName);
bool file_read(CString *cstr, const char *filepath);
bool file_getline(const char **start, CString *result);
bool file_write_len(const char *filepath, const char *str, int len);
#endif


