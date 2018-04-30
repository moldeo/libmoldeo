#!/bin/sh

DIE=0

./autogen.sh --prefix=/usr --enable-gstreamer=1.0

echo "deb directory..."
(mkdir deb ) > /dev/null || {
	echo "cleaning..."

	(rm -Rf deb/*) > /dev/null || {
	  echo
	  echo "**Error**: removing error ."
	  echo ""
	  exit 1
	}
}



echo "making distribution source file..."
(make dist) > /dev/null || {
  echo
  echo "**Error**: make dist ."
  echo ""
  exit 1
}


echo "copying..."

(mv libmoldeo-*.tar.gz ./deb/) > /dev/null || {
  echo
  echo "**Error**: moving file ."
  echo ""
  exit 1
}


echo "renaming..."
( rename 's/\.tar.gz/\.orig.tar.gz/' deb/*.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: renaming ."
  echo ""
  exit 1
}
( rename 's/\-/\_/' deb/*.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: renaming ."
  echo ""
  exit 1
}


echo "extracting..."

(tar -C deb -zxvf ./deb/*.orig.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: extracting ."
  echo ""
  exit 1
}

cd deb/libmoldeo-*
dh_make -l -e info@moldeo.org -p libmoldeo -c gpl3

sed -i -e 's/Architecture: any/Architecture: amd64/g' debian/control
sed -i -e 's/Maintainer: fabricio /Maintainer: Moldeo Interactive /g' debian/control
sed -i -e 's/<insert the upstream URL, if relevant>/http:\\\\www.moldeo.org/g' debian/control
sed -i -e 's/libmoldeoBROKEN/libmoldeo/g' debian/control
sed -i -e 's/,autotools-dev/, autotools-dev, libmuparser-dev, liblua5.1-0-dev, libtinyxml-dev, libsdl1.2-dev, libglew-dev, freeglut3-dev, libgle3-dev, ftgl-dev, libfreeimage-dev, libalut-dev, libgstreamer1.0-dev, libgstreamer-plugins-base1.0-dev, libboost-dev, libboost-filesystem-dev, libboost-thread-dev,  libboost-system-dev, autoconf-archive, check/g' debian/control

#development
sed -i -e '0,/, ${misc:Depends}/{s/, ${misc:Depends}/,${misc:Depends}, liblua5.1-0-dev, libgstreamer1.0-dev, libgstreamer-plugins-base1.0-dev, libglew-dev, libalut-dev/g}' debian/control
sed -i -e '0,/<insert up to 60 chars description>/{s/<insert up to 60 chars description>/Moldeo Core Library, Dev Package/}'  debian/control

#binary
sed -i -e 's/, ${misc:Depends}/,${misc:Depends}, liblua5.1-0-dev, libglew-dev, libsdl1.2debian, libfreeimage3, libalut0, libgstreamer1.0-0, libgstreamer-plugins-base1.0-0, gstreamer1.0-tools, libmuparser-dev, libboost-dev, libboost-filesystem-dev, libboost-system-dev, libboost-thread-dev, libtinyxml-dev/g' debian/control
sed -i -e 's/<insert up to 60 chars description>/Moldeo Core Library/'  debian/control
sed -i -e 's/<insert long description, indented with spaces>/Moldeo is a multimedia interactive framework based on C++, OpenGL, Gstreamer, Lua, Freeimage, muparser/g'  debian/control

sed -i -e 's/unstable/experimental/g' debian/changelog
sed -i -e 's/fabricio/Moldeo Interactive/g' debian/changelog
sed -i -e 's/Initial release (Closes: #nnnn)  <nnnn is the bug number of your ITP>/Initial release/g' debian/changelog
#sed -i -e 's///g'  debian/control
#
#sed -i -e 's///g' debian/control


echo "usr/lib/lib*.so.*" > debian/libmoldeo.install
echo "usr/lib/lib*.so" >> debian/libmoldeo.install
echo "usr/share/moldeo/*" >> debian/libmoldeo.install
echo "usr/share/moldeo/data/*" >> debian/libmoldeo.install
echo "usr/share/moldeo/data/icons/*" >> debian/libmoldeo.install
echo "usr/share/moldeo/data/fonts/*" >> debian/libmoldeo.install
echo "usr/share/moldeo/data/shaders/*" >> debian/libmoldeo.install


echo "usr/lib" > debian/libmoldeo.dirs
echo "usr/share/moldeo" >> debian/libmoldeo.dirs
echo "usr/share/moldeo/data" >> debian/libmoldeo.dirs
echo "usr/share/moldeo/data/icons" >> debian/libmoldeo.dirs
echo "usr/share/moldeo/data/fonts" >> debian/libmoldeo.dirs
echo "usr/share/moldeo/data/shaders" >> debian/libmoldeo.dirs

echo "usr/lib" > debian/libmoldeo-dev.dirs
echo "usr/include" >> debian/libmoldeo-dev.dirs
echo "usr/include/libmoldeo" >> debian/libmoldeo-dev.dirs

echo "usr/include/*" > debian/libmoldeo-dev.install
echo "usr/include/libmoldeo/*" >> debian/libmoldeo-dev.install
echo "usr/lib/pkgconfig/*" >> debian/libmoldeo-dev.install


xed ../../control.amd64.min17 debian/control ../../libmoldeo1.install.amd64 debian/libmoldeo.install ../../libmoldeo1.dirs.amd64 debian/libmoldeo.dirs ../../libmoldeo-dev.dirs.amd64 debian/libmoldeo-dev.dirs ../../libmoldeo-dev.install.amd64 debian/libmoldeo-dev.install debian/changelog

dpkg-buildpackage -us -uc -rfakeroot 2>&1 | tee ../../buildpkg_logs.txt


echo "Success: extraction"
exit 0
