INCLUDES                    = @CHECK_CFLAGS@
TESTS                       = check_libmoldeo
noinst_PROGRAMS             = $(TESTS)
check_libmoldeo_SOURCES      = check_libmoldeo.cpp
check_libmoldeo_LDADD        = @CHECK_LIBS@ \
	-L../libmoldeo -lmoldeo \
	@MUPARSER_LIBS@ \
	@SDL_LIBS@ \
	@GLE_LIBS@ \
	@FREEIMAGE_LIBS@ \
	@FREEALUT_LIBS@ \
	@GLEW_LIBS@ \
	@CG_LIBS@ \
	@CGGL_LIBS@ \
	@LUA51_LIBS@ \
	@GST_LIBS@ \
	@FT2_LIBS@
CLEANFILES                  = check_libmoldeo.log

