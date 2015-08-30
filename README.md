# libmoldeo
## Moldeo Library Core

**libmoldeo** is the core library of Moldeo framework.


### Compile and install

We recommend to visit [Moldeo Installer](http://github.com/moldeo/moldeoinstaller) to check the best way to compile and install the full platform.

In a linux terminal run:
```bash
    git clone http://github.com/moldeo/libmoldeo
    cd libmoldeo
    ./autogen.sh
    make
    sudo make install
```

### Documentation

* [Moldeo's Official Site](http://www.moldeo.org)
* [libmoldeo Documentation [english]](http://www.moldeo.org/documentation/libmoldeo/doc/en/html/index.html)
* [libmoldeo Documentation [spanish]](http://www.moldeo.org/documentation/libmoldeo/doc/es/html/index.html)

### Moldeo 1.0

**Moldeo** is an open source framework that allows the experimentation of new
artistic, scientific and communicational concepts with new computer graphics
technologies. It is an open source development that allows the creation of
interactive environments with 2D, 3D animation and video elements as well as
digital effects in real time.

### Open standards

**libmoldeo** is builded over other libraries and open source community code.

### Moldeo Platform Modules

Moldeo is already in the beta state but still in heavy development.

Moldeo Module | Description | Language | Dependencies
------------- | ----------- | -------- | ---------
[libmoldeo](http://github.com/moldeo/libmoldeo) | Core Library | C/C++ | Boost, OpenGL, muParser, FreeImage, GStreamer, 
[moldeoplayer](http://github.com/moldeo/moldeoplayer) | Player (command line project player) | C/C++ | SDL2, SDL1, GLUT
[moldeonet](http://github.com/moldeo/moldeonet) | Graphical User Interfaces (GUIs) | Javascript, HTML5 | libmoldeo, [nw.js](https://github.com/nwjs/nw.js/), d3.js, jQuery, bootstrap
[moldeoplugins](http://github.com/moldeo/moldeoplugins) | Plugins | C/C++ | libmoldeo...
[moldeodirector](http://github.com/moldeo/moldeodirector) | Interface Development Environment (IDE) | C/C++ | libmoldeo, wxWidgets
[moldeosamples](http://github.com/moldeo/moldeosamples) | Full examples | XML/Lua | libmoldeo, moldeoplayer
[moldeoinstaller](http://github.com/moldeo/moldeoinstaller) | Platform Installer for Linux, OSX | XML/Lua | All

### Community

* [Moldeo's Community](http://www.moldeo.org/community)
* [Moldeo's Facebook Community](http://www.facebook.com/comunidadmoldeo)



