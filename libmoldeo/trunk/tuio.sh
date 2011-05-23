#!/bin/sh

DIE=0

(wget http://prdownloads.sourceforge.net/reactivision/TUIO_CPP-1.4.zip?download -O TUIO_CPP.zip) > /dev/null || {
  echo
  echo "**Error**: Downloading http://prdownloads.sourceforge.net/reactivision/TUIO_CPP-1.4.zip?download ."
  echo "Get the C++ API source zip at http://www.tuio.org/?software"
  exit 1
}


echo
echo "Unzipping TUIO"

( unzip -o TUIO_CPP.zip ) > /dev/null || {
	echo 
	echo "**Error**: unzipping TUIO_CPP.zip "
	exit 1
}

( mv TUIO_CPP/Makefile Makefile.TUIO ) > /dev/null || {
	echo
	echo "**Error**: couldn't move TUIO_CPP/Makefile to Makefile.TUIO"
	exit 1
}

( patch Makefile.TUIO -i Makefile.TUIO.patch -o TUIO_CPP/Makefile ) > /dev/null || {
	echo
	echo "**Error**: couldn't patch TUIO_CPP/Makefile with Makefile.TUIO.patch"
	exit 1
}

echo "Making TUIO"

(( make all -C ./TUIO_CPP ) && (sudo make install -C ./TUIO_CPP ) ) || {
	echo
	echo "**Error**: trying to make all in TUIO_CPP"
	exit 1
}


if test "$DIE" -eq 1; then
  exit 1
fi

echo "Success: TUIO_CPP library generated"
exit 0
