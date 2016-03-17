AC_INIT([libmoldeo], [1.0.0.155], [info@moldeointeractive.com.ar], [libmoldeo])

AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)

AC_LIBTOOL_CONFIG

AC_PROG_CC
AC_PROG_CC_STDC
AC_C_CONST
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL

AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_CXX_C_O

dnl check for muParser
PKG_CHECK_MODULES([MUPARSER], [muparser] ,has_muparser=yes)

dnl check tinyxml
PKG_CHECK_MODULES([TINYXML], [tinyxml] ,has_tinyxml=yes)

dnl check for gtk+-2.0
PKG_CHECK_MODULES([GTK2], [gtk+-2.0], has_gtk2=yes )

dnl check for sdl
PKG_CHECK_MODULES([SDL], [sdl] ,has_sdl=yes)

dnl check for opengl
AC_ARG_WITH( [gles],
AS_HELP_STRING([--with-gles],
[use open gl es 2, default: no]),
[with_gles=yes])

host_name="default"

AC_ARG_ENABLE(host,
[--enable-host[=host name] Specify the host os name:
			64: 64 bit linux os [default]
			86: 32 bits linux os
            raspbian: RASPBIAN/debian 8 32 bits
            xosx: Mac OSX with mac ports],
[ if test "x$enableval" = "x86" ; then
	host_name="86"
  elif test "x$enableval" = "x64" ; then
	host_name="64"
  elif test "x$enableval" = "xraspbian" ; then
	host_name="raspbian"
	host_cflags="-I/opt/vc/include -DOPENGLESV2"
	host_ldflags="-L/opt/vc/lib"
  elif test "x$enableval" = "xosx" ; then
    host_name="osx"
	host_cflags="-I/opt/local/include "
	host_ldflags="-L/opt/local/lib -framework GLUT -framework OpenGL -framework Cocoa"
  else
	echo 
	echo "Error!"
	echo "Unknown host name"
	exit -1
  fi
])
AC_SUBST([HOSTCFLAGS],$host_cflags)
AC_SUBST([HOSTLDFLAGS],$host_ldflags)

if test "x$host_name" = "xosx" ; then
    PKG_CHECK_MODULES([LUA51], [lua-5.1] ,has_lua51=yes)
    AC_SUBST([GLUT_LIBS], [])
    AC_SUBST([GL_LIBS], [])
    AC_SUBST([GLE_LIBS], [])
    
    dnl check for glew
    AC_CHECK_HEADER(GL/glew.h, , AC_MSG_ERROR([glew header not found.]))
    AC_CHECK_LIB(GLEW, glewInit, AC_SUBST([GL_LIBS], [-lGLEW]), AC_MSG_ERROR([glew not found.]) )
else
    PKG_CHECK_MODULES([LUA51], [lua5.1] ,has_lua51=yes)
    AS_IF([test "x$with_gles" = "xyes"],
        [PKG_CHECK_MODULES([GL], [glesv2, egl] ,has_glesv2=yes)],
        [PKG_CHECK_MODULES([GL], [gl, glew] ,has_gl=yes)])
    
    dnl check for glut
    AC_CHECK_HEADER(GL/glut.h, , AC_MSG_ERROR([glut header not found.]))
    AC_CHECK_LIB(glut, glutMainLoop, AC_SUBST([GLUT_LIBS], [-lglut]), AC_MSG_ERROR([glut not found.]), )

fi


dnl check for freeimage
AC_CHECK_HEADER(FreeImage.h, , AC_MSG_ERROR([FreeImage header not found.]))
AC_CHECK_LIB(freeimage, FreeImage_GetVersion, AC_SUBST([FREEIMAGE_LIBS], [-lfreeimage]), AC_MSG_ERROR([FreeImage not found.]), )


#dnl check for openal
#PKG_CHECK_MODULES([OPENAL], [openal] ,has_openal=yes)

dnl check for glu
#PKG_CHECK_MODULES([GLU], [glu] ,has_glu=yes)


dnl check for gle
#AC_CHECK_HEADER(GL/gle.h, , AC_MSG_ERROR([gle header not found.]))
#AC_CHECK_LIB(gle, gleDestroyGC, AC_SUBST([GLE_LIBS], [-lgle]), AC_MSG_ERROR([gle not found.]), )

dnl check for ftgl
PKG_CHECK_MODULES([FTGL], [ftgl], has_ftgl=yes )


dnl check for alut
#PKG_CHECK_MODULES([FREEALUT], [freealut] ,has_freealut=yes)

dnl check for gstreamer
GST_VERSION_MAJOR=0
GST_VERSION_MINOR=10
GST_VERSION_RELEASE=19
GSTPLUG_VERSION_RELEASE=7
GSTREAMER_REQ=$GST_VERSION_MAJOR.$GST_VERSION_MINOR.$GST_VERSION_RELEASE
GSTPLUGINS_REQ=$GST_VERSION_MAJOR.$GST_VERSION_MINOR.$GSTPLUG_VERSION_RELEASE
GST_MAJORMINOR=$GST_VERSION_MAJOR.$GST_VERSION_MINOR

PKG_CHECK_MODULES(GST, gstreamer-$GST_MAJORMINOR >= $GSTREAMER_REQ,
  HAVE_GSTREAMER=yes)
AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_LIBS)

PKG_CHECK_MODULES(GST_INTERFACES, gstreamer-interfaces-$GST_MAJORMINOR >= $GSTREAMER_REQ,
  HAVE_GSTREAMER_INTERFACES=yes)
AC_SUBST(GST_INTERFACES_CFLAGS)
AC_SUBST(GST_INTERFACES_LIBS)

dnl check for truetype
AC_CHECK_FT2

dnl check for check test suite
PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])

dnl check boost libraries
AX_BOOST_BASE
AX_BOOST_ASIO
AX_BOOST_THREAD
AX_BOOST_FILESYSTEM
AX_BOOST_SYSTEM

dnl autoscan results
AC_CHECK_FUNCS([floor])
AC_CHECK_FUNCS([getcwd])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([rint])
AC_CHECK_FUNCS([sqrt])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strstr])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([float.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_TYPES([ptrdiff_t])
AC_C_INLINE
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_VPRINTF
AC_HEADER_STDBOOL
AC_HEADER_STDC
AC_PREREQ
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT
AC_STRUCT_TM
AC_TYPE_SIZE_T

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")


dnl Additional arguments (and substitutions)
dnl Allow the pkg-config directory to be set
AC_ARG_WITH(pkgconfigdir,
	AC_HELP_STRING([--with-pkgconfigdir],
	[Use the specified pkgconfig dir (default is libdir/pkgconfig)]),
	[pkgconfigdir=${withval}],
	[pkgconfigdir='${libdir}/pkgconfig'])

AC_SUBST([pkgconfigdir])


echo "GLES: $with_gles"
echo "HOST: $host_name"
echo "HOSTCFLAGS: $host_cflags"
echo "HOSTLDFLAGS: $host_ldflags"
echo "DEBUG: $debug"

dnl Makefile outputs
AC_OUTPUT([
Makefile
data/Makefile
libmoldeo/Makefile
libmoldeotest/Makefile
libmoldeo.pc
libmoldeotest.pc
test-suite/Makefile
])

