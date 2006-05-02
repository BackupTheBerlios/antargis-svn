GLee
GL Easy Extension Library
Version 3.03

By Ben Woodhouse
http://elf-stone.com


LICENSE

Copyright (c)2004  Ben Woodhouse  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are 
met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer as
the first lines of this file unmodified.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY BEN WOODHOUSE ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL BEN WOODHOUSE BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

This licence includes and incorporates the SGI FREE SOFTWARE LICENSE B 
(Version 1.1 [02/22/2000]) full details of which can be found at 
http://oss.sgi.com/projects/FreeB.

For the avoidance of doubt, this work falls within the definition of a 
'Larger Work' within that licence, since it incorporates parts of SGI's
glExt.h, wglExt.h and glxExt.h header files, which form part of SGI's
OpenGL Sample Implementation. 



CONTACT

Send any general questions, bug reports etc to me (Ben Woodhouse):
  ben [at] elf-stone.com

Martin Büchler is now the GLee maintainer for Linux, so direct 
linux-specific enqiries to him:
  lousyphreak [at] gmx.at
  


DESCRIPTION

GLee provides a simple interface for using extensions and core OpenGL 
functionality beyond OpenGL version 1.1, and automates the otherwise tedious 
process of linking function pointers. GLee works with both C and C++ 
compilers (as of version 2.3).

Because the code is automatically generated, the latest extensions can 
be included rapidly in new versions. Currently there is support 
for OpenGL up to 1.5 and almost all registered extensions. For a complete 
list of extensions, please see the accompanying extensionList.txt file. 

For extension specifications, please visit: 

   http://oss.sgi.com/projects/ogl-sample/registry/



HOW TO INSTALL 

MSVC binary version

  1. Copy GLee.lib to your visual C++ lib directory 
  (eg C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\Lib)

  2. Copy GLee.h to your visual C++ include\GL directory 
  (eg C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\include\gl)
  

Linux binary version (GLee-*-bin.tar.gz)
  (Installation has to be done as root)

  1. unpack GLee-*-bin.tar.gz
  (tar -xzf GLee-2.0-bin.tar.gz)

  2. Install with the inst script
  (cd glee && ./inst)

Linux source version (GLee-*-src.tar.gz)
  1. unpack GLee-*-src.tar.gz
  (tar -xzf GLee-2.0-src.tar.gz)

  2. cd to "glee" and run the gnu build chain
  (cd glee && ./configure && make)

  3. Become root (password needed)
  (su)

  4. Install the lib
  (make install)
  

HOW TO USE

Include glee.h in your project. If you're not using the binary
version then you'll also need to add glee.c and glee.h to
your project. 

Add a call to GLeeInit() after your OpenGL initialisation code. 
If the return code is true then the initialisation is successful. 
From this point forwards, you should have access to all OpenGL 
extensions and core functionality. 

You can query the availability of an extension like this:

if (GLEE_ARB_point_parameters)
{
   //GL_ARB_point_parameters is supported
   glPointParameterfARB(...);
}

WGL extensions have a GLEE_WGL_ prefix. For example, 
GLEE_WGL_ARB_pbuffer. GLX extensions work in a similar way. 

You can also query the OpenGL version:
if (GLEE_VERSION_1_3)
{
  //OpenGL 1.3 is supported 
  glLoadTransposeMatrixf(...) ; 
  ...
}

If GLeeInit returns false, you can get more detailed error 
information by calling GLeeGetErrorString(). This returns a pointer 
to a string which contains error description(s). 

At any time you can call GLeeGetExtStrGL() or GLeeGetExtStrWGL() 
(win32) or GLeeGetExtStrGLX() (linux) to retrieve a pointer to the 
corresponding extension list string.  



LINUX NOTES - by LousyPhreak

The binary version was compiled with gcc 3.3 and was also tested 
with gcc 3.2, but if you still have gcc 2.x you need the source 
version. If you don't know which version you have on your system just 
look at the output of 'gcc --version'.

You should be using the NVIDIA headers if you have the NVIDIA drivers
installed, and the MESA headers otherwise.

One more note:
The binary version might complain about missing glXGetProcAddressARB
on the linker stage, but if you get this error, use the source 
version and email me (lousyphreak [at] gmx.at).

Compiling on linux:
You just need to replace the linking of libGL by libGLee:
gcc main.cpp whatever.cpp -lGLee -o myproject

Everything else should be the same as on windows.


CREDITS

Big thanks to Martin Büchler (LousyPhreak on the GameDev.net forums) for 
a huge amount of testing, tweaking, suggestions and advice to get GLee to 
work properly with Linux. The cross-platform version really would not have 
been possible without him. 

An honourable mention for Promit, for being yet another person to suggest a 
cross-platform version of GLee, and for providing some code, which, although 
it didn't work with GLee, was none the less the inspiration for the cross 
platform version of GLee. 

Thanks to Daniel Jo (Ostsol on the OpenGL.org forums) for helping to get  
GLEE 2.1 working with the OGLSL extensions (ARB_shader_objects, 
ARB_vertex_shader and ARB_fragment_shader). 

Also thanks to everyone on the gamedev.net OpenGL forum who gave suggestions 
or reported bugs. 


CHANGES

3.03  : Another linux compatibility bug fixed

3.02  : GLX typedef bug fixed

3.01  : Minor linux compatibility bugs fixed

3.0   : Removed STL and other C++ specific features to make GLee 
        compatible with C compilers 
        
        Added a number of WGL extensions that had previously been
        excluded due to a bug with GLeeGen (now fixed)

2.21  : Fixed VC6 compilation bug

2.2   : Added full OpenGL 1.5 support

2.1   : Added the OpenGL shading language extensions: 
        ARB_shader_objects, ARB_vertex_shader and ARB_fragment_shader. 
		
2.01  : Fixed missing description comment in header
        Fixed include guard

2.0   : Removed dependency on glext.h and wglext.h
        Added cross-platform code for support with linux      
        Fixed potential stability problems 

1.2   : Made library compatible with VC7 and VC7.1

1.12  : Cleaned up some commenting errors in GLeeGen

1.11  : Added functions to get WGL and GL extension strings.
        Fixed minor formatting errors caused by a bug in GLeeGen (now fixed)

1.1   : Added detailed error checking. 
        Fixed possible buffer overrun issue 

1.01  : GLeeInit no longer requires a device context handle.