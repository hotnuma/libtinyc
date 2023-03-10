project(
    'tinyc',
    ['c'],
    version : '1.0',
    license : 'LGPL-2.0',
    default_options : [
        'c_std=c99',
    ],
)

c_args = [
    '-Wall',
    '-Wextra',
    '-D_DEFAULT_SOURCE',
    '-D__STDC_FORMAT_MACROS',
]

if get_option('buildtype') == 'plain'
  c_args += '-O2'
endif

tinyc_deps = [
    dependency('gtk+-3.0'),
    dependency('libpcre'),
]

tinyc_headers = [
    'lib/cdirent.h',
    'lib/cdirparser.h',
    'lib/cfile.h',
    'lib/cfileinfo.h',
    'lib/cinifile.h',
    'lib/clist.h',
    'lib/cprocess.h',
    'lib/cregexp.h',
    'lib/cstring.h',
    'lib/cstringlist.h',
    'lib/etkaction.h',
    'lib/etype.h',
    'lib/libapp.h',
    'lib/libconv.h',
    'lib/libhtml.h',
    'lib/libmacros.h',
    'lib/libpath.h',
    'lib/libstr.h',
    'lib/libtest.h',
    'lib/print.h',
]

tinyc_sources = [
    'lib/cdirent.c',
    'lib/cdirparser.c',
    'lib/cfile.c',
    'lib/cfileinfo.c',
    'lib/cinifile.c',
    'lib/clist.c',
    'lib/cprocess.c',
    'lib/cregexp.c',
    'lib/cstring.c',
    'lib/cstringlist.c',
    'lib/etkaction.c',
    'lib/libapp.c',
    'lib/libconv.c',
    'lib/libhtml.c',
    'lib/libpath.c',
    'lib/libstr.c',
    'lib/libtest.c',
    'lib/print.c',
]

# Build and install library ---------------------------------------------------

tinyc = static_library(
    'tinyc',
    c_args : c_args,
    dependencies : tinyc_deps,
    sources : tinyc_sources,
    install : true,
)

install_headers(tinyc_headers, subdir: 'tinyc')

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : tinyc,
                 version : '1.0',
                 name : 'libtinyc',
                 filebase : 'tinyc',
                 subdirs : 'tinyc',
                 description : 'A simple C library.')

# Build test program --------------------------------------------------------

tinytest_sources = [
    'test/test_cdirent.c',
    'test/test_cdirparser.c',
    'test/test_cfile.c',
    'test/test_cfileinfo.c',
    'test/test_cinifile.c',
    'test/test_clist.c',
    'test/test_cprocess.c',
    'test/test_cstring.c',
    'test/test_cstringlist.c',
    'test/test_libapp.c',
    'test/test_libhtml.c',
    'test/test_libpath.c',
    'test/test_libstr.c',
    'test_main.c',
]

tinytst = executable(
    'tinytest',
    include_directories : 'lib',
    c_args : c_args,
    sources : tinytest_sources,
    link_with : [tinyc],
)

test('tinytest', tinytst)

# Build sample program --------------------------------------------------------

tinycmd_sources = [
    'main.c',
]

executable(
    'tinycmd',
    include_directories : 'lib',
    c_args : c_args,
    sources : tinycmd_sources,
    link_with : [tinyc],
)


