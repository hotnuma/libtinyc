#ifndef CDIRPARSER_H
#define CDIRPARSER_H

#include "cstring.h"

#define CDP_NODE     0
#define CDP_RELATIVE (1 << 0)
#define CDP_SUBDIRS  (1 << 1)
#define CDP_DIRS     (1 << 2)
#define CDP_FILES    (1 << 3)

typedef struct _CDirParser CDirParser;

CDirParser* cdirparser_new();
bool cdirparser_open(CDirParser *parser, const char *directory, int flags);
void cdirparser_close(CDirParser *parser);
void cdirparser_free(CDirParser *parser);

// auto free ------------------------------------------------------------------

#define CDirParserAuto GC_CLEANUP(_freeCDirParser) CDirParser
GC_UNUSED static inline void _freeCDirParser(CDirParser **parser)
{
    //print("_freeCDirParser");

    cdirparser_free(*parser);
}

bool cdirparser_read(CDirParser *parser, CString *filepath /*, int* type*/);


#endif // CDIRPARSER_H


