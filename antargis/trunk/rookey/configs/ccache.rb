module Rookey
  class CcacheConfig<Configurator
    provides :compilerAddon
    needs :compiler
    
    def run(config)
      ccache=searchProgram("ccache")
      if ccache
        config["CCACHE"]=ccache
      end
    end
  end
end