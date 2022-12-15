#include "CFile.h"

#include <stdlib.h>
#include <string.h>

CFile::CFile()
{
}

CFile::~CFile()
{
    close();
}

bool CFile::open(const char *filepath, const char *mode)
{
    close();

    _fp = fopen(filepath, mode);

    return (_fp != nullptr);
}

void CFile::flush()
{
    if (_fp)
        fflush(_fp);
}

void CFile::close()
{
    if (_fp)
        fclose(_fp);

    _fp = nullptr;
    _curr = nullptr;
}

bool CFile::read(const char *filepath)
{
    if (!open(filepath, "rb"))
        return false;

    fseek(_fp, 0, SEEK_END);
    unsigned long size = ftell(_fp);
    fseek(_fp, 0, SEEK_SET);

    _buffer.resize(size + 1);

    if (fread(_buffer.data(), 1, size, _fp) != size)
    {
        close();

        _buffer.clear();
        _curr = nullptr;

        return false;
    }

    close();

    _buffer.terminate(size);
    _curr = _buffer.data();

    return true;
}

void CFile::write(const char *str)
{
    if (!_fp || !str)
        return;

    fwrite(str, strlen(str), 1, _fp);
}

// static write.
bool CFile::write(const char *filepath, const CString &buffer)
{
    FILE *fp = fopen(filepath, "wb");

    if (!fp)
        return false;

    const char *buff = buffer.c_str();
    size_t size = buffer.size();

    size_t ret = fwrite(buff, 1, size, fp);

    if (ret != size)
    {
        fclose(fp);
        return false;
    }

    fflush(fp);
    fclose(fp);

    return true;
}

void CFile::operator<<(const char *str)
{
    if (!_fp || !str)
        return;

    fwrite(str, strlen(str), 1, _fp);
}

bool CFile::getLine(CString &result)
{
    if (!_curr)
        return false;

    return strGetLine(&_curr, result);
}


