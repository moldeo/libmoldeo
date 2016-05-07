#!/bin/sh

DIE=0

./autogen.sh --prefix=/usr

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
dh_make -l -e info@moldeointeractive.com.ar -p libmoldeo

gedit ../../control.amd64.min17 debian/control ../../libmoldeo1.install.amd64 debian/libmoldeo.install ../../libmoldeo1.dirs.amd64 debian/libmoldeo.dirs ../../libmoldeo-dev.dirs.amd64 debian/libmoldeo-dev.dirs ../../libmoldeo-dev.install.amd64 debian/libmoldeo-dev.install debian/changelog

echo " 
 cd deb/libmoldeo-*
 dpkg-buildpackage -us -uc -rfakeroot 2>&1 | tee ../../buildpkg_logs.txt



"


echo "Success: extraction"
exit 0
