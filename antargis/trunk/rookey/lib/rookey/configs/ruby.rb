
module Rookey
	  class RubyConfig<Configurator
      provides :ruby
      if ROOKEY_CONFIG[:use_mkmf]==true or ROOKEY_CONFIG[:use_mkmf].nil? 
        #require 'mkmf'
        provides :compiler
		    
		    def run(config)
		      
		      
		      require 'mkmf'
          
          @config=CONFIG
		      
			      config.add("INCLUDEDIRS",get("archdir")+" "+get("includedir"))
			      config.add("CFLAGS",get("CFLAGS"))
			      config.add("LDFLAGS",get("LIBS"))
			      config.add("LDFLAGS",get("LIBRUBYARG"))
			      config.add("LDSHAREDFLAGS",get("LDSHARED").sub(/^[^ ]+ /,""))
			      config["DLEXT"]=get("DLEXT")
			      config["host_os"]=get("host_os")      
			      config["GPP_BASE"]="g++"
			      config["GCC_BASE"]="gcc"
			
			      {"CPP"=>"GPP_BASE","CC"=>"GCC_BASE"}.each{|name,base|      
			        config[name]=searchProgram(config[base])
			      }
	          
	          if config["host_os"]=~/darwin.*/ and false
	            pgc="-finstrument-functions"
	            pgl="-finstrument-functions -lSaturn"
	          else
	            pgc="-pg"
	            pgl="-pg"
	          end 
	          config.add("CFLAGS",pgc) if ROOKEY_CONFIG[:profile]
	          config.add("LDFLAGS",pgl) if ROOKEY_CONFIG[:profile]
	          
	          ["CFLAGS","CPPFLAGS","LDFLAGS"].each{|name|
	            config.add(name,ENV[name]) if ENV[name]
	          }
	          
		    end
		    
		    
      elsif ROOKEY_CONFIG[:extconfig]=='mingw32'
        def run(config)
          puts "OK"
          #install("ruby")
          #exit
          prefix=File.join(File.split(File.split(__FILE__)[0])[0],"externals","build")
          @config=readConfig(File.join(prefix,"lib","ruby","1.8","i386-mingw32","rbconfig.rb"))
          @config["prefix"]=prefix
          puts "OK"
          pp @config
          
          config.add("INCLUDEDIRS",get("archdir")+" "+get("includedir"))
          puts "OK"
          pp config
          config.add("CFLAGS",get("CFLAGS").gsub("-g",""))
          config.add("LDFLAGS",get("LIBS"))
          config.add("LDFLAGS",get("LIBRUBYARG"))
          config.add("LDSHAREDFLAGS",get("LDSHARED").sub(/^[^ ]+ /,""))
          
          pp config
          puts "OK"
          config
          #exit
        end
        
        def readConfig(filename)
          config={}
          File.open(filename).read.split("\n").each{|line|
            if line=~/.*CONFIG\[.*\].*/
              k,v=line.scan(/.*CONFIG\["(.*)"\] = "(.*)".*/)[0]
              config[k]=v if v and k
            end
          }
          config
        end
	  end
    private
      # recurse through ruby's CONFIG structure 
      def get(n)
        return "" if n.nil?
        v=@config[n]
        return "" if v.nil?
        v.gsub(/\$\(([^)]*\))/) {|a|
          f=a[2..-2]
          get(f)
        }      
      end
    
	end
end