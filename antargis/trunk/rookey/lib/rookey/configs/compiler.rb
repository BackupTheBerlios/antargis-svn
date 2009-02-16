module Rookey
  class CompilerConfig<Configurator
    needs :compiler
    
    def run(config)
      config.add("INCLUDEDIRS",File.join(File.split(File.split(__FILE__)[0])[0],"cpp"))
      config.add("LDFLAGS","-lMallocDebug") if ROOKEY_CONFIG[:malloc_debug]
    end
  end
end