require 'mkmf'
require File.join(File.split(__FILE__)[0],"ruby.rb")

module Rookey
  class GLConfig<RubyConfig
    provides :opengl
    
    def run(config)
      case get("host_os")
        when /darwin/
          config.add("INCLUDEDIRS","/usr/X11/include")        
          config.add("LDFLAGS","-Wl,-framework,OpenGL")
      end      
    end   
  end 
end