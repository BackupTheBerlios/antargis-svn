require 'mkmf'
require File.join(File.split(__FILE__)[0],"ruby.rb")

module Rookey
  class ZConfig<Configurator
    provides :z
    needs :compiler
    
    def run(config)
      unless checkLibrary(config,"z","compress")
        install("zlib")
      end
    end   
  end 
end