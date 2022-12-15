TEMPLATE = app
CONFIG = console c99
INCLUDEPATH = lib
DEFINES =

#TARGET = tinycmd
TARGET = tinytest

HEADERS = \
    lib/clist.h \
    lib/cstring.h \
    lib/cstringlist.h \
    lib/libtest.h \
    lib/print.h \

SOURCES = \
    lib/clist.c \
    lib/cstring.c \
    lib/cstringlist.c \
    lib/libtest.c \
    lib/print.c \
    0Temp.c \
    main.c \
    test/test_cstring.c \
    test_main.c

DISTFILES = \
    install.sh \
    meson.build \
    Readme.md \


