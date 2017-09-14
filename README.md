MDMTrace and Oxford Sim files

Note that everything currently used by the T4T collaboration
is in the MDMTrace subdir.


*** COMPILING MDM TRACE - RAYTRACE CODE ***

Compilation creates a shared library file, libMDMTrace.so, and an executable
mdmTraceApp, in the 'build' directory. These can optionally be installed in
the ${CMAKE_INSTALL_PREFIX}/lib and ${CMAKE_INSTALL_PREFIX}/bin directories.

The following shows how to compile a fresh version of the repository, assuming
desired installation locations of ~/install/lib and ~/install/bin  If you want
another prefix, modify the -DCMAKE_INSTALL_PREFIX=${HOME}/install part
accordingly. If you don't care about installing the package and are happy to
always run it from the build directory, omit the -DCMAKE_INSTALL_PREFIX part
entirely.

> cd MDMTrace
> mkdir -p build
> cd build
> cmake -DCMAKE_INSTALL_PREFIX=${HOME}/install ../
> ## ^^ Or just: 'cmake ../ ' if you don't care about installing the package
> make
> make install <<--- Optional: Copies library to ${CMAKE_INSTALL_PREFIX}/lib
                               and executable to ${CMAKE_INSTALL_PREFIX}/bin
