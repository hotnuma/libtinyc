#ifndef CFILE_H
#define CFILE_H

#include "CString.h"
#include <stdio.h>

class CFile
{
public:

    CFile();
    ~CFile();

    bool open(const char *filepath, const char *mode = "rb");
    void flush();
    void close();

    bool read(const char *filepath);
    void write(const char *str);
    static bool write(const char *filepath, const CString &buffer);
    void operator<<(const char *str);

    bool getLine(CString &result);

    CString& buffer() {return _buffer;}
    operator const char*() const {return _buffer.c_str();}

    const char* c_str() const {return _buffer.c_str();}
    char* data() const {return _buffer.data();}
    int size() const {return _buffer.size();}

private:

    CString _buffer;

    FILE *_fp = nullptr;
    char *_curr = nullptr;

};

#endif // CFILE_H


