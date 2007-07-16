#!/bin/bash

#prepare()
#{

mkdir -p usr
mkdir -p usr/bin
mkdir -p usr/include
mkdir -p usr/lib

mkdir tmp

# unzip zip-files
for a in libs/*.zip ; {
    cd tmp
	unzip -qou ../${a}
    cd ..
}

# extract tar.gz-files
for a in libs/*.gz ; {
    cd tmp
	tar xfz ../${a}
    cd ..
}

for a in `find -name "*.dll" ` ; {
    cp $a usr/bin 2>/dev/null
}

for a in `find -name "SDL*.h" ` ; {
    cp $a usr/include 2>/dev/null
}

for a in begin_code.h close_code.h zlib.h zconf.h png.h pngconf.h; {
    cp `find -name $a` usr/include
}


for a in `find -name "*.a" ` ; {
    cp $a usr/lib 2>/dev/null
}

for a in `find -name "*.lib" ` ; {
    cp $a usr/lib 2>/dev/null
}
for a in usr/lib/*.lib ; {
    cp $a `dirname $a``basename "$a"|sed -e "s/\.lib$/\.dll\.a/"`
}
for a in usr/lib/*.dll.a ; {
    cp $a `dirname $a``basename "$a"|sed -e "s/^/lib/"`
}

for a in `find -name "*.exe";find -name "*-config"` ; {
    cp $a usr/bin 2>/dev/null
}

mv usr/bin/i386-mingw32msvc-sdl-config usr/bin/sdl-config

#}
dir=$(pwd)




# build ruby
cd tmp
tar xfz ../libs/ruby-1.8.6.tar.gz
cd ruby-1.8.6
        ./configure --prefix=/usr --enable-debug --enable-install-doc
####                # patch fake.rb
#####                cat fake.rb |sed -e "s/\"\\\\\";/\"\\\\\"\";/">fake.tmp
#####                mv fake.tmp fake.rb
                make
    export DESTDIR=../../
                make install
    export DESTDIR=""
cd ..

# install rant
tar xfz ../libs/rant-0.5.8.tar.gz
cd rant-0.5.8
../../usr/bin/ruby.exe install.rb


cd ..
cd ..

