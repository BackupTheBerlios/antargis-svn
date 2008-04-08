module Rookey
  class DebugConfig<Configurator
    needs :compiler
    
    def run(config)
      pp ROOKEY_CONFIG
      if ROOKEY_CONFIG[:debug]
	      config.add("CFLAGS","-O0 -g")
	      config.add("LDFLAGS","-g")
      end
    end
  end
end