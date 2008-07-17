require 'mkmf'
require File.join(File.split(__FILE__)[0],"ruby.rb")

module Rookey
  class GLConfig<Configurator
    provides :opengl
    needs :compiler
    
    def run(config)
      case config["host_os"]
        when /darwin/
          config.add("INCLUDEDIRS","/usr/X11/include")        
          config.add("LDFLAGS","-Wl,-framework,OpenGL")
        when /win32/
          #config.add("INCLUDEDIRS","/usr/X11/include")        
          config.add("LDFLAGS","-lopengl32 -lglu32")
        else
          config.add("LDFLAGS","-lGL -lGLU")
      end      
    end   
  end 
end
