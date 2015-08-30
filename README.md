# libmoldeo
## Moldeo Library Core

libmoldeo is the core system of Moldeo framework.

## Moldeo 1.0

**Moldeo** is an open source framework that allows the experimentation of new
artistic, scientific and communicational concepts with new computer graphics
technologies. It is an open source development that allows the development of
interactive environments with 2D, 3D animation and video elements as well as
digital effects in real time.

### Open standards

**libmoldeo** is builded over other libraries and open source community code.

### Moldeo Platform Modules

Module | Description | Language | Dependencies
------ | ----------- | -------- | ---------
libmoldeo | Core Library | C/C++ | Boost, OpenGL, muParser, FreeImage
moldeoplayer | Player (command line project player) | C/C++ | SDL2, SDL1, GLUT
moldeonet | Graphical User Interface (GUI) | Javascript, HTML5 | libmoldeo, [nw.js](https://github.com/nwjs/nw.js/), d3.js, jQuery, bootstrap
moldeoplugins | Plugins | C/C++ | libmoldeo...
moldeodirector | Interface Development Interface (IDE) | C/C++ | libmoldeo, wxWidgets

### Documentation

[Moldeo's Official Site](http://www.moldeo.org).
[libmoldeo Documentation [english]](http://www.moldeo.org/documentation/libmoldeo/doc/en/html/index.html).
[libmoldeo Documentation [spanish]](http://www.moldeo.org/documentation/libmoldeo/doc/es/html/index.html).


## Installation

In a linux terminal run:
	git clone http://github.com/moldeo/libmoldeo
	cd libmoldeo
	./autogen.sh
	make
	sudo make install


We recommend to visit http://github.com/moldeo/moldeoinstaller to check the best way to compile and install the full software

