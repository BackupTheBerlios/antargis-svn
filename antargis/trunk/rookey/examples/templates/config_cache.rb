module Rookey
  @@config=Marshal.load("o:Rookey::Config:
@hash{\"CC\"/usr/bin/gcc\"SWIG_OPTIONS\" -Wall -DAGEXPORT\"CCACHE\"/opt/local/bin/ccache\"	SWIG\" /opt/local/bin/swig\"CPP\"/usr/bin/g++\"CFLAGS\"O -arch ppc -arch i386 -Os -pipe -fno-common -D_GNU_SOURCE=1 -D_THREAD_SAFE\"host_os\"darwin9.0\"
DLEXT\"bundle\"INCLUDEDIRS\"ÿ /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/1.8/universal-darwin9.0 /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/include /usr/X11/include /Library/Ruby/Gems/1.8/gems/rookey-0.1/lib/rookey/./cpp /opt/local/include/SDL\"LDFLAGS\"“ -lpthread -ldl -lm  -lruby -Wl,-framework,OpenGL -lz -L/opt/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa -lSDL_image -lSDL_ttf -lSDL_mixer -lpng\"GCC_BASE\"gcc\"GPP_BASE\"g++\"LDSHAREDFLAGS\"B -arch ppc -arch i386 -pipe -bundle -undefined dynamic_lookup")
end
