module Rookey
  class SwigConfig<Configurator
    provides :swig
    
    def run(config)
      # FIXME: search program
      config.add("SWIG_OPTIONS","-Wall -DAGEXPORT")
    end
    
  end
end