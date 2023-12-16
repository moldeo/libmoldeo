AC_INIT([libmoldeo], [1.4.0.4], [info@moldeointeractive.com.ar], [libmoldeo])

AC_CANONICAL_HOST
AC_CANONICAL_BUILD
AC_CANONICAL_TARGET
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

dnl Set install paths.
if test "x$prefix" = "xNONE" ; then
  prefix="/usr/local"
fi
if test "x$exec_prefix" = "xNONE" ; then
  exec_prefix=$prefix
fi

echo $host

case "$host" in
        *-*-mingw*|*-*-cygwin*)
                AC_DEFINE(PLATFORM_WIN32, 1, [Platform is Win32])
                PLATFORM_STRING="Win32"
                ;;
        x86_64-*linux*)
                with_boost_libdir="/usr/lib/x86_64-linux-gnu"
                PLATFORM_STRING="Linux"
                ;;
        *-*-linux*)
                AC_DEFINE(PLATFORM_LINUX, 1, [Platform is Linux])
                PLATFORM_STRING="Linux"
                ;;
        *-*-darwin*)
		AC_MSG_WARN([Hello])
                AC_DEFINE(PLATFORM_APPLE, 1, [Platform is Apple])
                PLATFORM_STRING="Apple"
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac checks!])
                ;;
esac


case "$host" in
        i?86-* | k?-* | athlon-* | pentium*-)
                AC_DEFINE(ARCH_X86, 1, [Architecture is x86])
                ARCH_SPECIFIC_CFLAGS=""
                ARCH_STRING="X86"
                ;;
        x86_64-*)
                AC_DEFINE(ARCH_X86_64, 1, [Architecture is x86-64])
                ARCH_SPECIFIC_CFLAGS="-DUSE_ADDR64"
                ARCH_STRING="X86-64"
                libdir=$prefix"/lib"
                corefoundation='/System/Library/Frameworks/CoreFoundation.framework/Versions/A/Headers'
                ;;
        aarch64-*-darwin*)
                AC_DEFINE(OS_DARWIN, 1, [Darwin backend])
                AC_SUBST(OS_DARWIN)
                ARCH_SPECIFIC_CFLAGS=""
                ARCH_STRING="arm64"
                libdir=$prefix"/lib"
                corefoundation='/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/CoreFoundation.framework/Versions/A/Headers'
                ;;
        armv7l-*)
                AC_DEFINE(ARCH_X86_64, 1, [Architecture is armv7l x86-64])
                ARCH_SPECIFIC_CFLAGS="-DUSE_ADDR64"
                ARCH_STRING="X86-64"
                libdir=$prefix"/lib"
                ;;
        ppc-* | powerpc-*)
		AC_MSG_WARN([HI THERE!])
                AC_DEFINE(ARCH_PPC, 1, [Architecture is PowerPC])
                ARCH_SPECIFIC_CFLAGS=""
                ARCH_STRING="PowerPC"
                ;;
        *)
                AC_MSG_ERROR([Unknown Architecture])
                ;;
esac


dnl check for muParser
PKG_CHECK_MODULES([MUPARSER], [muparser] ,has_muparser=yes)

dnl check tinyxml
PKG_CHECK_MODULES([TINYXML], [tinyxml] ,has_tinyxml=yes)

dnl check for gtk+-2.0
#PKG_CHECK_MODULES([GTK2], [gtk+-2.0], has_gtk2=yes )

dnl check for sdl
PKG_CHECK_MODULES([SDL], [sdl] ,has_sdl=yes)

dnl check for opengl
AC_ARG_WITH( [gles],
AS_HELP_STRING([--with-gles],
[use open gl es 2, default: no]),
[with_gles=yes])

host_name="default"
librequires='lua5.1, tinyxml, muparser, sdl >= 1.2.0, ftgl, gl, glew, freetype2, check'
OPENGLESV2=''
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
	host_cflags="-I/opt/vc/include"
	host_ldflags="-L/opt/vc/lib"
    	librequires='lua5.1, tinyxml, muparser, sdl >= 1.2.0, ftgl, egl, glesv2, freetype2, check'
	OPENGLESV2='#define OPENGLESV2 1'
	with_gles=yes
  elif test "x$enableval" = "xosx" ; then
    host_name="osx"
	host_cflags="-std=c++20 -I/opt/local/include -I"$corefoundation
	host_ldflags="-L/opt/local/lib -framework GLUT -framework OpenGL -framework Cocoa -framework CoreFoundation"
    librequires='glew, lua-5.1, tinyxml, muparser, sdl >= 1.2.0, ftgl, freetype2, check'
  else
	echo
	echo "Error!"
	echo "Unknown host name"
	exit -1
  fi
])

AC_SUBST([OPENGLESV2],$OPENGLESV2)

GST_VERSION_MAJOR=1
GST_VERSION_MINOR=0
GST_VERSION_RELEASE=0
GSTPLUG_VERSION_RELEASE=0

AC_ARG_ENABLE(gstreamer,
[--enable-gstreamer[=gstreamer version] Specify the gstreamer version:
			1.0: Gstreamer version 1.0 [default]
      0.10: Gstreamer version 0.10 ],
[ if test "x$enableval" = "x0.10" ; then
    GST_VERSION_MAJOR=0
    GST_VERSION_MINOR=10
    GST_VERSION_RELEASE=19
    GSTPLUG_VERSION_RELEASE=7
  fi
])


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

dnl check for ftgl
PKG_CHECK_MODULES([FTGL], [ftgl], has_ftgl=yes )

dnl check for gstreamer
GSTREAMER_REQ=$GST_VERSION_MAJOR.$GST_VERSION_MINOR.$GST_VERSION_RELEASE
GSTPLUGINS_REQ=$GST_VERSION_MAJOR.$GST_VERSION_MINOR.$GSTPLUG_VERSION_RELEASE
GST_MAJORMINOR=$GST_VERSION_MAJOR.$GST_VERSION_MINOR

if test "x$host_name" = "xosx" ; then

AC_SUBST(GST_CFLAGS,"-I/Library/Frameworks/GStreamer.framework/Versions/1.0/Headers")
AC_SUBST(GST_LIBS,"-F /Library/Frameworks -framework GStreamer")

if test "x$GST_VERSION_MAJOR" = "x1" ; then
	host_cflags="$host_cflags -DGSTVERSION"
fi

else

PKG_CHECK_MODULES(GST, gstreamer-$GST_MAJORMINOR >= $GSTREAMER_REQ,
HAVE_GSTREAMER=yes)
AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_LIBS)

if test "x$GST_VERSION_MAJOR" = "x1" ; then
    librequires="$librequires , gstreamer-1.0, gstreamer-app-1.0"
    host_cflags="$host_cflags -DGSTVERSION"
    PKG_CHECK_MODULES(GST_INTERFACES, gstreamer-app-$GST_MAJORMINOR >= $GSTREAMER_REQ, HAVE_GSTREAMER_INTERFACES=yes)
elif test "x$GST_VERSION_MAJOR" = "x0" ; then
    librequires="$librequires , gstreamer-0.10, gstreamer-interfaces-0.10, gstreamer-app-0.10"
    PKG_CHECK_MODULES(GST_INTERFACES, gstreamer-interfaces-$GST_MAJORMINOR >= $GSTREAMER_REQ, HAVE_GSTREAMER_INTERFACES=yes)
fi

AC_SUBST(GST_INTERFACES_CFLAGS)
AC_SUBST(GST_INTERFACES_LIBS)

fi


AC_SUBST([HOSTCFLAGS],$host_cflags)
AC_SUBST([HOSTLDFLAGS],$host_ldflags)


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

AC_SUBST([librequires])

dnl Additional arguments (and substitutions)
dnl Allow the pkg-config directory to be set
AC_ARG_WITH(pkgconfigdir,
	AC_HELP_STRING([--with-pkgconfigdir],
	[Use the specified pkgconfig dir (default is libdir/pkgconfig)]),
	[pkgconfigdir=${withval}],
	[pkgconfigdir='${libdir}/pkgconfig'])

AC_SUBST([pkgconfigdir])

echo "PLATFORM_STRING: $PLATFORM_STRING"
echo "ARCH_STRING: $ARCH_STRING"
echo "GLES: $with_gles"
echo "OPENGLESV2: $OPENGLESV2"
echo "GSTREAMER VERSION: $GSTREAMER_REQ"
echo "librequires: $librequires"
echo "HOST NAME: $host_name"
echo "HOST: $host"
echo "HOSTCFLAGS: $host_cflags"
echo "HOSTLDFLAGS: $host_ldflags"
echo "DEBUG: $debug"

dnl Makefile outputs
AC_OUTPUT([
Makefile
data/Makefile
libmoldeo/moTypes.h
libmoldeo/Makefile
libmoldeotest/Makefile
libmoldeo.pc
libmoldeotest.pc
test-suite/Makefile
])
