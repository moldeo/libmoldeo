ACLOCAL_AMFLAGS=-I m4

if DEBUG
DFLAGS = -g3 -O0 -fpermissive
else
DFLAGS = -O2 -fpermissive
endif

datadir = ${prefix}/share/moldeo/data
modulesdir = ${libdir}/moldeo
docdir = ${prefix}/share/doc/moldeo

AM_CPPFLAGS = ${DFLAGS} ${HOSTCFLAGS} $(all_includes) -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\" -I../libmoldeo
AM_CXXFLAGS = ${AM_CPPFLAGS}
AM_LDFLAGS = ${HOSTLDFLAGS} -L../libmoldeo -lmoldeo

TESTS                       = check_libmoldeo
noinst_PROGRAMS             = $(TESTS)
check_libmoldeo_SOURCES     = check_libmoldeo.cpp
check_libmoldeo_CPPFLAGS    = ${CHECK_CFLAGS} \
	${FT2_CFLAGS} \
	${GST_CFLAGS} \
	${GL_CFLAGS} \
	$(GST_INTERFACES_CFLAGS) \
	${MUPARSER_CFLAGS} \
	${BOOST_CPPFLAGS} \
	${SDL_CFLAGS} \
	${LUA51_CFLAGS}

check_libmoldeo_LDADD       = ${CHECK_LIBS} \
	${FT2_LIBS} \
	${GST_LIBS} \
	${GL_LIBS} \
	$(GST_INTERFACES_LIBS) \
	${BOOST_ASIO_LIB} \
	${BOOST_THREAD_LIB} \
	${BOOST_FILESYSTEM_LIB} \
	${LUA51_LIBS}


CLEANFILES                  = check_libmoldeo.log


