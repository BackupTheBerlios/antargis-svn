require 'mkmf'
require File.join(File.split(__FILE__)[0],"ruby.rb")

module Rookey
  class PNGConfig<Configurator
    provides :png
    needs :compiler
    needs :z
        
    def run(config)
      unless checkLibrary(config,"png","png_get_io_ptr")
        install("png")
        checkLibrary(config,"png","png_get_io_ptr")
      end
    end   
  end 
end