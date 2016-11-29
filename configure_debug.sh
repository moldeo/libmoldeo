$ mkdir debug
$ mkdir release
$ cd debug && /path/to/configure --prefix=/dbg \
   CPPFLAGS=-DDEBUG CXXFLAGS="-g3 -O0" && make && make install
$ cd ../release && /path/to/configure CPPFLAGS=-DNDEBUG && make && make install

