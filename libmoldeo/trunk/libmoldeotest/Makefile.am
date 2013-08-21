ACLOCAL_AMFLAGS=-I m4

if DEBUG
AM_CFLAGS = -g3 -O0
AM_CPPFLAGS = -g3 -O0
AM_CXXFLAGS = -g3 -O0
else
AM_CFLAGS = -O2
AM_CPPFLAGS = -O2
AM_CXXFLAGS = -O2
endif

datadir = ${prefix}/share/moldeo
modulesdir = ${libdir}/moldeo
docdir = ${prefix}/share/doc/moldeo

AM_CPPFLAGS = ${AM_CPPCFLAGS} $(all_includes) -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"
AM_CXXFLAGS = ${AM_CXXCFLAGS} $(all_includes) -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"

lib_LTLIBRARIES = libmoldeotest.la
libmoldeotest_la_SOURCES = \
	moText.cpp \
	moTypes.cpp

libmoldeotest_la_CPPFLAGS = ${FT2_CFLAGS} \
	${GST_CFLAGS} \
	${GTK2_CFLAGS} \
	${MUPARSER_CFLAGS} \
	${BOOST_CPPFLAGS} \
	${SDL_CFLAGS} \
	${LUA51_CFLAGS} 

libmoldeotest_la_LIBADD = -ldl\
	${FT2_LIBS} \
	${GTK2_LIBS} \
	${SDL_LIBS} \
	${TINYXML_LIBS} \
	${GL_LIBS} \
	${GLE_LIBS} \
	${GLEW_LIBS} \
	${FTGL_LIBS} \
	${GLUT_LIBS} \
	${GLU_LIBS} \
	${FREEALUT_LIBS} \
	${FREEIMAGE_LIBS} \
	${MUPARSER_LIBS} \
	${BOOST_ASIO_LIB} \
	${BOOST_THREAD_LIB} \
	${BOOST_FILESYSTEM_LIB} \
	${BOOST_SYSTEM_LIB} \
	${LUA51_LIBS} \
	${GST_LIBS} 

libmoldeotest_includedir = $(includedir)/libmoldeotest

libmoldeotest_include_HEADERS = \
	moldeotest.h \
	moArray.h \
	moArrayH.h \
	moPort.h \
	moText.h \
	moTypes.h 

METASOURCES = AUTO

