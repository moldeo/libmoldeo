AC_INIT(libmoldeo,0,7)

AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)

AC_LIBTOOL_CONFIG

AC_PROG_CC
AC_PROG_CC_STDC
AC_C_CONST
AC_PROG_LIBTOOL

AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_CXX_C_O

dnl check for muParser
PKG_CHECK_MODULES([MUPARSER], [muparser] ,has_muparser=yes)

dnl check for lua51
PKG_CHECK_MODULES([LUA51], [lua5.1] ,has_lua51=yes)

dnl check for sdl
PKG_CHECK_MODULES([SDL], [sdl] ,has_sdl=yes)

dnl check tinyxml
PKG_CHECK_MODULES([TINYXML], [tinyxml] ,has_tinyxml=yes)

dnl check for sdl-net
AC_CHECK_HEADER(SDL/SDL_net.h, , AC_MSG_ERROR([SDL_net header not found.]))
AC_CHECK_LIB(SDL_net, SDLNet_Init, AC_SUBST([SDL_NET_LIBS], [-lSDL_net]), AC_MSG_ERROR([SDL_net not found.]), [${SDL_LIBS} ${WIN32_LIBS}])

dnl check for sdl-image
AC_CHECK_HEADER(SDL/SDL_image.h, , AC_MSG_ERROR([SDL_image header not found.]))
AC_CHECK_LIB(SDL_image, IMG_Linked_Version, AC_SUBST([SDL_IMAGE_LIBS], [-lSDL_image]), AC_MSG_ERROR([SDL_image not found.]), [${SDL_LIBS} ${WIN32_LIBS}])

dnl check for opengl
AC_CHECK_HEADER(GL/gl.h, , AC_MSG_ERROR([opengl header not found.]))
AC_CHECK_LIB(GL, glEnd, AC_SUBST([GL_LIBS], [-lGL]), AC_MSG_ERROR([opengl not found.]), )

dnl check for glew
AC_CHECK_HEADER(GL/glew.h, , AC_MSG_ERROR([glew header not found.]))
AC_CHECK_LIB(GLEW, glEnd, AC_SUBST([GLEW_LIBS], [-lGLEW]), AC_MSG_ERROR([glew not found.]), )

dnl check for glut
AC_CHECK_HEADER(GL/glut.h, , AC_MSG_ERROR([glut header not found.]))
AC_CHECK_LIB(glut, glutMainLoop, AC_SUBST([GLUT_LIBS], [-lglut]), AC_MSG_ERROR([glut not found.]), )

dnl check for gle
AC_CHECK_HEADER(GL/gle.h, , AC_MSG_ERROR([gle header not found.]))
AC_CHECK_LIB(gle, gleDestroyGC, AC_SUBST([GLE_LIBS], [-lgle]), AC_MSG_ERROR([gle not found.]), )

dnl check for ftgl
PKG_CHECK_MODULES([FTGL], [ftgl] ,has_ftgl=yes)

dnl check for freeimage
AC_CHECK_HEADER(FreeImage.h, , AC_MSG_ERROR([FreeImage header not found.]))
AC_CHECK_LIB(freeimage, FreeImage_GetVersion, AC_SUBST([FREEIMAGE_LIBS], [-lfreeimage]), AC_MSG_ERROR([FreeImage not found.]), )

dnl check for alut
PKG_CHECK_MODULES([FREEALUT], [freealut] ,has_freealut=yes)

dnl nvidia-cG-toolkit requires pthread
AC_CHECK_LIB(pthread, pthread_attr_init, LIBS="$LIBS -lpthread")

dnl check for cg
AC_LANG_PUSH(C++)dnl
AC_CHECK_LIB(Cg, cgCreateContext,
[
	AC_CHECK_HEADERS(Cg/cg.h,
	[
		AC_DEFINE(HAVE_CG, 1, [define if you have libcg])
		AC_SUBST([CG_LIBS])
		CG_LIBS="-lCg"
		have_cg=yes
	],
	[
		AC_MSG_ERROR([cG header file missing or not in Cg/ include subdir])
	],
	)

],
[
	AC_MSG_ERROR([
	* nvidia cG toolkit is missing.
	* You can download it from: http://developer.nvidia.com/object/cg_toolkit.html ])
],
)

AC_CHECK_LIB(CgGL, cgGLBindProgram,
[
	AC_CHECK_HEADERS(Cg/cgGL.h,
	[
		AC_DEFINE(HAVE_CGGL, 1, [define if you have libcg])
		AC_SUBST([CGGL_LIBS])
		CGGL_LIBS="-lCgGL -lGL"
		have_cggl=yes
	],
	[
		AC_MSG_ERROR([cGGL header file missing or not in Cg/ include subdir])
	])

],
[
	AC_MSG_ERROR([
	* Something's wrong with nvidia cG toolkit
	* Probably bad installation
	* You can download it from: http://developer.nvidia.com/object/cg_toolkit.html ])

],
[ -lGL ]
)
AC_LANG_POP(C++)

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

dnl check for truetype
AC_CHECK_FT2

dnl check for check test suite
PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])

dnl check boost libraries
AX_BOOST_BASE
AX_BOOST_ASIO
AX_BOOST_THREAD
AX_BOOST_FILESYSTEM

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

dnl Additional arguments (and substitutions)
dnl Allow the pkg-config directory to be set
AC_ARG_WITH(pkgconfigdir,
	AC_HELP_STRING([--with-pkgconfigdir],
	[Use the specified pkgconfig dir (default is libdir/pkgconfig)]),
	[pkgconfigdir=${withval}],
	[pkgconfigdir='${libdir}/pkgconfig'])

dnl Makefile outputs
AC_OUTPUT([
Makefile
libmoldeo/Makefile
test-suite/Makefile
])

