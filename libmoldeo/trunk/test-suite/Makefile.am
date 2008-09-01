INCLUDES                    = @CHECK_CFLAGS@
TESTS                       = check_libmoldeo
noinst_PROGRAMS             = $(TESTS)
check_libmoldeo_SOURCES      = check_libmoldeo.cpp
check_libmoldeo_LDADD        = @CHECK_LIBS@ \
	-L../libmoldeo -lmoldeo \
	-lGL -lGLU -lSDL -lfreeimage -lopenal -lGLEW -lmuparser -lCg -lCgGL -lgle\
	@LUA51_LIBS@ \
	@GST_LIBS@ \
	@FT2_LIBS@
CLEANFILES                  = check_libmoldeo.log

