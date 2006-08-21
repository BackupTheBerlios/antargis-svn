#!/bin/bash

TOPDIR=`pwd`

baselibs="tiff-3.8.2-1 zlib-1.2.3 libpng-1.2.8 jpeg-6b-4 freetype-2.1.10"

SDL="SDL-1.2.11.tar.gz"
SDLIMAGE="SDL_image-1.2.5.tar.gz"
SDLMIXER="SDL_mixer-1.2.7.tar.gz"
SDLTTF="SDL_ttf-2.0.8.tar.gz"
RUBY="ruby-1.8.4.tar.gz"
SMPEG="smpeg-0.4.4.tar.gz"

OGG="libogg-1.1.3.tar.gz"
VORBIS="libvorbis-1.1.2.tar.gz"

export CFLAGS="-I${TOPDIR}/include"

download_file()
{
	if test ! -e $1 ; then
		echo "try download $2"
    wget $2
  fi
}


download_files()
{
	download_file "${SMPEG}" "ftp://sunsite.dk/pub/os/linux/loki/open-source/smpeg/${SMPEG}"

	download_file "${OGG}" "http://downloads.xiph.org/releases/ogg/${OGG}"
	download_file "${VORBIS}" "http://downloads.xiph.org/releases/vorbis/${VORBIS}"

	for a in ${baselibs}; {
		download_file "${a}-bin.zip" "http://belnet.dl.sourceforge.net/sourceforge/gnuwin32/${a}-bin.zip"
		download_file "${a}-lib.zip" "http://belnet.dl.sourceforge.net/sourceforge/gnuwin32/${a}-lib.zip"
	}

	download_file "${SDL}" "http://www.libsdl.org/release/${SDL}"
	download_file "${SDLIMAGE}" "http://www.libsdl.org/projects/SDL_image/release/${SDLIMAGE}"
	download_file "${SDLMIXER}" "http://www.libsdl.org/projects/SDL_mixer/release/${SDLMIXER}"
	download_file "${SDLTTF}" "http://www.libsdl.org/projects/SDL_ttf/release/${SDLTTF}"

	download_file "${RUBY}" "ftp://ftp.ruby-lang.org/pub/ruby/ruby-1.8.4.tar.gz"

}

build_libsdl()
{
  if test ! -f ${TOPDIR}/usr/lib/libSDL.a ; then
		tar xfz SDL-1.2.11.tar.gz
		cd `echo "${SDL}"|sed -e "s/\.tar\.gz//"`
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc
		make
		make install
		cd ..
	fi
}

build_libsdl_image()
{
  if test ! -f ${TOPDIR}/usr/lib/libSDL_image.a ; then
		echo "=========================="
		echo "LIBSDL IMAGE"
		echo "=========================="
	
		tar xfz ${SDLIMAGE}
		cd `echo "${SDLIMAGE}"|sed -e "s/\.tar\.gz//"`
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc
		make
		make install
		cd ..
	fi
}

build_libsdl_ttf()
{  
	if test ! -f ${TOPDIR}/usr/lib/libSDL_ttf.a ; then
	
		echo "=========================="
		echo "LIBSDL TTF"
		echo "=========================="
	
		tar xfz ${SDLTTF}
		cd `echo "${SDLTTF}"|sed -e "s/\.tar\.gz//"`
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc || exit
		make
		make install
		cd ..
	fi
}


build_libsdl_mixer()
{
  if test ! -f ${TOPDIR}/usr/lib/libSDL_mixer.a ; then
	
		echo "=========================="
		echo "LIBSDL MIXER"
		echo "=========================="
	
		tar xfz ${SDLMIXER}
		cd `echo "${SDLMIXER}"|sed -e "s/\.tar\.gz//"`
		export CFLAGS="-I${TOPDIR}/usr/include"
		echo ${CFLAGS}
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc --disable-music-ogg-shared --enable-music-ogg
		make
		make install
		cd ..
	fi
}

build_smpeg()
{
  if test ! -f ${TOPDIR}/usr/lib/libsmpeg.a ; then
	
		echo "=========================="
		echo "SMPEG"
		echo "=========================="
	
		tar xfz ${SMPEG}
		cd `echo "${SMPEG}"|sed -e "s/\.tar\.gz//"`
		export CFLAGS="-I${TOPDIR}/usr/include"
		export LDFLAGS="-L${TOPDIR}/usr/lib -L${TOPDIR}/usr/bin"
		export CXX="i586-mingw32msvc-g++"
		export CC="i586-mingw32msvc-g++"
		export RANLIB="i586-mingw32msvc-ranlib"
		echo ${CFLAGS}
		echo ${CXX}
		rm config.cache
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc --with-sdl-prefix=${TOPDIR}/usr --disable-shared --enable-static
		make
		make install
		cd ..
		export CFLAGS=""
		export LDFLAGS=""
		export CXX=""
		export CC=""
		export RANLIB=""
	fi
}

build_ruby()
{
  if test ! -f ${TOPDIR}/usr/bin/ruby.exe ; then
		echo "=========================="
		echo "RUBY"
		echo "=========================="
	
		tar xfz ${RUBY}
		cd `echo "${RUBY}"|sed -e "s/\.tar\.gz//"`
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc --enable-debug
		make
		make install
		cd ..
	fi
}


do_test()
{
	i586-mingw32msvc-g++ -o sdl_image_test.exe "-Wl,-(" -I/home/david/xcompile/contrib/usr/include/SDL -I/home/david/xcompile/contrib/usr/include -D_GNU_SOURCE=1 -Dmain=SDL_main -L/home/david/xcompile/contrib/usr/lib -lmingw32 -lSDLmain -lSDL -mwindows sdl_image_test.cc -lSDL_image
}

install_baselibs()
{
	cd usr	
	for a in ${baselibs} ; {
		unzip -u ../${a}-bin.zip
		unzip -u ../${a}-lib.zip
	}
	cd ..
}

build_ogg()
{
  if test ! -f ${TOPDIR}/usr/lib/libvorbis.a ; then
	
		echo "=========================="
		echo "ogg"
		echo "=========================="
	
		tar xfz ${OGG}
		cd `echo "${OGG}"|sed -e "s/\.tar\.gz//"`
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc --disable-docs --disable-shared
		make
		make install
		cd ..

		echo "=========================="
		echo "vorbis"
		echo "=========================="

		export LDFLAGS="-logg -L${TOPDIR}/usr/lib"
	
		tar xfz ${VORBIS}
		cd `echo "${VORBIS}"|sed -e "s/\.tar\.gz//"`
		./configure --prefix=${TOPDIR}/usr --host=i586-mingw32msvc --target=i586-mingw32msvc --with-pic --enable-static --disable-docs --disable-shared
		make
		make install
		cd ..

	fi
	
}


export CFLAGS="-I${TOPDIR}/usr/include"

download_files
mkdir usr

install_baselibs
build_ogg
build_libsdl
build_libsdl_image

build_smpeg

build_libsdl_ttf
build_libsdl_ttf
build_libsdl_mixer

build_ruby

do_test
