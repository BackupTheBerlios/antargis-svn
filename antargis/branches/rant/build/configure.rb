#
# This is a library for simple build-configuration management.
# For a brief example have a look at BoA's configure-script.
# 
# The configuration-process goes like this
# 1) include this file
# 2) define cmd-line-options with CFG.addOption
# 3) add checks for programs, files, libraries (checkProgram, checkFile, checkLibrary)
# 4) add custom-checks with addCheck(...)
# 5) include basic-configuration functionality with CFG.includeConfig - have a look at build/configs/unix.rb for more information on this
# 6) run the configuration with CFG.run - a config.rb file will be written 
##

require 'build/platform.rb'

module CFG
	@@options=[]
	@@config={}
	@@checks=[]

	def CFG.addOption(name,short,help,param=nil,default=nil,&proc)
		@@options.push({:name=>name,:short=>short,:help=>help,:proc=>proc,:param=>param,:default=>default})
	end
	def CFG.options
		@@options
	end
	def CFG.addCheck(name,needed=true,&proc)
		@@checks << {:name=>name,:proc=>proc,:needed=>needed}
	end

	def CFG.call(name)
		@@options.each{|op|
			if op[:name]==name || op[:short]==name
				op[:proc].call
			end
		}
	end
	def CFG.parseArgs
		ARGV.each{|arg|
			found=false
			# parse single arguments like "-d" or "-dgh" (like tar -xfz)
			if arg=~/^-[a-z]+$/
				found=true
				# check if all characters represent a correct option
				arg[1..-1].scan(/./){|byte|
					found=false if @@options.select{|op|op[:short]==byte}.length==0
				}
				if found
					# all were ok, so call each these options
					arg[1..-1].scan(/./){|byte|
						@@options.each{|op|op[:proc].call if op[:short]==byte}
					}
				end
			end
			if not found
				# option could not yet be processed, so do a second try
				@@options.each{|op|
					name=op[:name]
					ename=name.gsub("+","\\\\+")
					#puts "ENAME:#{ename}"
					# parse "simple" options without parameters
					if arg=~/^--#{ename}$/ or arg=~/^-#{op[:short]}$/
						found=true
						op[:proc].call
					# parse options with parameter, given like this : "-d=<somedir>" or "--dir=bladir"
					elsif arg=~/^--#{ename}=.+/ or arg=~/^-#{op[:short]}=.+$/
						found=true
						op[:proc].call(arg.gsub(/^[^=]+=/,""))
					end
				}
			end
			puts "ERROR: argument '#{arg}' could not be processed!" if not found
		}
	end

	def CFG.checkDefaults
		@@options.each{|op|
			if get(op[:name]).nil? and op[:default]
				set(op[:name],op[:default])
			end
		}
	end

	def CFG.run
		parseArgs
		checkDefaults
		ok=runChecks
		saveConfig if ok
	end

	def CFG.runChecks
		puts 
		failed=[]
		@@checks.each{|c|
			text="checking "+c[:name]+" ..."
			print text
			ok=c[:proc].call
			#ok=(not ok.nil?)
			print " "*(40-text.length)
			puts ({true=>"ok",false=>"failed"}[ok])
			failed << c[:name] unless ok or not c[:needed]
		}
		if failed.length>0
			puts 
			puts "ERROR:"
			puts "The following tests failed:"
			failed.each{|f|puts f}
		end

		failed.length==0
	end

	def CFG.saveConfig
		configName="config.rb"
		f=File.open(configName,"w")
		f.puts <<EOT
$config={
#{@@config.collect{|k,v|"  '"+k+"'=>"+toS(v)}.join(",\n")}
}
EOT
		f.close
		#puts File.open(configName).read
	
	end

	def CFG.toS(v)
		case v
			when String
				"'"+v.to_s+"'"
			else
				v.to_s
		end
	end

	def CFG.set(n,v)
		@@config[n]=v
	end
	def CFG.get(n)
		@@config[n]
	end

	def CFG.checkProgram(program,needed=true)
		addOption("path-"+program,"",
			"set path to program '#{program}' like this:\n --path-#{program}=/usr/local/bin/#{program}","path") do |d|
				set(program,d)
			end

		addCheck("program "+program,needed) do
			path=get(program)
			path||=findProgram(program)
			r=testProgram(path)
			set(program,path) if r
			r
		end
	end

    def CFG.getPath
        p=ENV['PATH']
        psep={"/"=>":","\\"=>";"}[Dir.separator]
        #puts p,p.class,psep
        ps=p.split(psep)
        #puts get("prefix")
        ps << get("prefix")+Dir.separator+"bin" if get("prefix")
        if Dir.separator=="\\"
            ps << (Dir.pwd+"/build/win32/usr/bin").gsub("/",Dir.separator)
        end
        ps
    end

	def CFG.findProgram(program)
        #program+=".exe" if Dir.separator=="\\" and not program=~/\.\{exe|com|bat\}$/
        paths=getPath
        paths.each{|p|
            currentPath=p+Dir.separator+program
            #puts "TST #{currentPath}"
            if File.exists?(currentPath)
	#                puts "FOUND! at #{currentPath}"
                return currentPath
            end
        }
        return findProgram(program+".exe") if Dir.separator=="\\" and not program=~/exe$/
        return ""
		#`whereis #{program}`.gsub(/[^:]*: */,"").split(" ")[0]
	end

	def CFG.testProgram(path)
		#puts path,File.exists?(path),"--"
			File.exists?(path)
	end

	def CFG.includeConfig
		avail=["unix","mingw32"]
		addOption("base-config","",
			"set base-config like "+avail.join(", "),"config") do |v|
			set("base-config",v)
		end
		addCheck ("base-config") do ||
			c=get("base-config")
			c=nil unless avail.member?(c)
			c||="unix"
			require "build/configs/"+c+".rb"
			$config.each{|k,v|@@config[k]||=v}
			true
		end
	end

	def CFG.checkCompile
		
	end

	def CFG.checkLibrary(libname)
	end


	addOption("help","h",
		"display help text") do
			CFG.options.each{|op|
				text=""
				text << "--#{op[:name]}"
				text << "=<#{op[:param]}>" if op[:param]
				text << ", -#{op[:short]}" if op[:short]!=""
				text << "=<#{op[:param]}>" if op[:short]!="" and op[:param]
				text+=" "*(40-text.length)
				l=text.length
				helpa=op[:help].split("\n")
				text+=helpa[0]
				helpa[1..-1].each{|h|
					text << "\n"+" "*l+h
				}
				puts text
			}
	end
end
