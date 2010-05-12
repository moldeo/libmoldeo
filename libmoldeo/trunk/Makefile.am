ACLOCAL_AMFLAGS=-I m4
SUBDIRS = libmoldeo test-suite

EXTRA_DIST = AUTHORS ChangeLog NEWS README

# pkg-config stuff, note that libpng.pc is always required in order
# to get the correct library
pkgconfigdir = @pkgconfigdir@
pkgconfig_DATA = libmoldeo.pc

datadir = ${prefix}/share/moldeo
modulesdir = ${libdir}/moldeo
docdir = ${prefix}/share/doc/moldeo

dist_doc_DATA = README
dist_modules_DATA = README.modules
dist_data_DATA = README.data moldeologo.3ds Tuffy.ttf arial.ttf

AM_CPPFLAGS = -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"

