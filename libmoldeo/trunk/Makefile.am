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
preeffectsdir = ${modulesdir}/preeffects
effectsdir = ${modulesdir}/effects
posteffectsdir = ${modulesdir}/posteffects
iodevicesdir = ${modulesdir}/iodevices
resourcesdir = ${modulesdir}/resources

dist_doc_DATA = README
dist_modules_DATA = README.modules
dist_data_DATA = README.data arial.ttf moldeologo.3ds 
dist_preeffects_DATA = 
dist_effects_DATA = 
dist_posteffects_DATA = 
dist_iodevices_DATA = 
dist_resources_DATA = 

AM_CPPFLAGS = -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"

