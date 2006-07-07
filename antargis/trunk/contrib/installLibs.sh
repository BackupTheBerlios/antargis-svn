#!/bin/bash

TOPDIR=`pwd`

download_file()
{
	if test ! -f $1 ; then
    wget $2
  fi
}


download_files()
{
	download_file "SDL-1.2.11.tar.gz" "http://www.libsdl.org/release/SDL-1.2.11.tar.gz"
	download_file "SDL_image-1.2.5.tar.gz" "http://www.libsdl.org/projects/SDL_image/release/SDL_image-1.2.5.tar.gz"

	download_file "libpng-1.2.8-bin.zip" "http://heanet.dl.sourceforge.net/gnuwin32/libpng-1.2.8-bin.zip"
	download_file "jpegsrc.v6b.tar.gz" "http://www.ijg.org/files/jpegsrc.v6b.tar.gz"
}

build_libsdl()
{
  if test ! -f ${TOPDIR}/usr/lib/libSDL.a ; then
		tar xfz SDL-1.2.11.tar.gz
		cd SDL-1.2.11
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc
		make
		make install
		cd ..
	fi
}

build_libsdl_image()
{
	echo "=========================="
	echo "LIBSDL IMAGE"
	echo "=========================="

  tar xfz SDL_image-1.2.5.tar.gz
  cd SDL_image-1.2.5
	./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc
	make
	make install
	cd ..
}

install_libpng()
{
	echo "installing libpng"
	cd usr
	unzip ../libpng-1.2.8.bin.zip
	mv bin/libpng13.dll lib/libpng.dll
	cd ..
}

install_libjpeg()
{
	tar xfz jpegsrc.v6b.tar.gz
	cd jpeg-6b
	./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc --enable-shared
	make
	make install
	cd ..
	
}


download_files

build_libsdl
install_libpng
install_libjpeg
#build_libsdl_image