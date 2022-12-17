TEMPLATE = app
CONFIG = console c99
INCLUDEPATH = lib
DEFINES = _BSD_SOURCE __STDC_FORMAT_MACROS

TARGET = tinycmd
#TARGET = tinytest

HEADERS = \
    lib/cdirent.h \
    lib/cdirparser.h \
    lib/cfile.h \
    lib/cfileinfo.h \
    lib/cinifile.h \
    lib/ciniline.h \
    lib/cinisection.h \
    lib/clist.h \
    lib/cmacro.h \
    lib/cprocess.h \
    lib/cregexp.h \
    lib/cstring.h \
    lib/cstringlist.h \
    lib/libapp.h \
    lib/libconv.h \
    lib/libpath.h \
    lib/libstr.h \
    lib/libtest.h \
    lib/print.h \
    test_main.h \

SOURCES = \
    lib/cdirent.c \
    lib/cdirparser.c \
    lib/cfile.c \
    lib/cfileinfo.c \
    lib/cinifile.c \
    lib/ciniline.c \
    lib/cinisection.c \
    lib/clist.c \
    lib/cprocess.c \
    lib/cregexp.c \
    lib/cstring.c \
    lib/cstringlist.c \
    lib/libapp.c \
    lib/libconv.c \
    lib/libpath.c \
    lib/libstr.c \
    lib/libtest.c \
    lib/print.c \
    0Temp.c \
    main.c \
    test/test_cdirent.c \
    test/test_cdirparser.c \
    test/test_cfile.c \
    test/test_cfileinfo.c \
    test/test_cinifile.c \
    test/test_clist.c \
    test/test_cprocess.c \
    test/test_cstring.c \
    test/test_cstringlist.c \
    test/test_libapp.c \
    test/test_libpath.c \
    test/test_libstr.c \
    test_main.c \

DISTFILES = \
    install.sh \
    meson.build \
    Readme.md \


