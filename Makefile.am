ACLOCAL_AMFLAGS=-I m4
SUBDIRS = data libmoldeo libmoldeotest test-suite

EXTRA_DIST = AUTHORS ChangeLog NEWS README Makefile.TUIO.patch tuio.sh buildpkg.sh control.amd64 libmoldeo-dev.install.amd64 libmoldeo-dev.dirs.amd64 libmoldeo1.install.amd64 libmoldeo1.dirs.amd64 moldeoversion.txt

# pkg-config stuff
# to get the correct library
pkgconfigdir = @pkgconfigdir@
pkgconfig_DATA = libmoldeo.pc libmoldeotest.pc

datadir = ${prefix}/share/moldeo/data
modulesdir = ${libdir}/moldeo
docdir = ${prefix}/share/doc/moldeo
preeffectsdir = ${modulesdir}/preeffects
effectsdir = ${modulesdir}/effects
posteffectsdir = ${modulesdir}/posteffects
iodevicesdir = ${modulesdir}/iodevices
resourcesdir = ${modulesdir}/resources
moldeoversiondir = ${prefix}/share/moldeo

dist_doc_DATA = README
dist_modules_DATA = README.modules
dist_preeffects_DATA = 
dist_effects_DATA = 
dist_posteffects_DATA = 
dist_iodevices_DATA = 
dist_resources_DATA = 
dist_moldeoversion_DATA = moldeoversion.txt

AM_CPPFLAGS = -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"

