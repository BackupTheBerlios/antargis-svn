module Rookey
  class SwigConfig<Configurator
    provides :swig
    VERSIONS=["1.3.33","1.3.34","1.3.35"]
    
    def run(config,firstrun=true)
      swigs=searchPrograms("swig").select{|swig|checkVersion(swig)}
      if swigs.length==0
        puts "ERROR: no SWIG with version in #{VERSIONS.join(", ")} not found!"
        if firstrun
          install("swig")
	        run(config,false)
        end
      else
	      swig=swigs[0]
	      # FIXME: search program
	      config.add("SWIG_OPTIONS","-Wall -DAGEXPORT")
	      config.add("SWIG",swig)
      end
    end
    
    private
    def checkVersion(swig)
      run="#{swig} -version"
      output=`#{run}`
      VERSIONS.select{|v|output=~/#{v}/}.length>0
    end
  end
end