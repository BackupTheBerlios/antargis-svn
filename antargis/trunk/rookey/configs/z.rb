require 'mkmf'
require File.join(File.split(__FILE__)[0],"ruby.rb")

module Rookey
  class ZConfig<RubyConfig
    provides :z
    needs :compiler
    
    def run(config)
      checkLibrary(config,"z","compress")
    end   
  end 
end