#!/bin/sh

DIE=0
echo "cleaning..."
(rm -Rf deb/*) > /dev/null || {
  echo
  echo "**Error**: removing error ."
  echo ""
  exit 1
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


echo " 
Now execute in deb folder:
 dh_make -l -e fabricio.costa@moldeointeractive.com.ar
 cp ../../control.amd64 debian/control
 cp ../../rules.amd64 debian/rules
 cp ../../libmoldeo-dev.install.amd64 debian/libmoldeo-dev.install
 dpkg-buildpackage -us -uc -rfakeroot 2>&1 | tee ../../buildpkg_logs.txt
"


echo "Success: extraction"
exit 0
