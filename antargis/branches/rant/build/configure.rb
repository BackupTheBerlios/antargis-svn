#
# This is a library for simple build-configuration management.
# For a brief example have a look at BoA's configure-script (link:files/configure.html)
# This file may contain some examples how to use this.
# 
# The configuration-process goes like this
#  1) include this file
#  2) define cmd-line-options with CFG.addOption
#  3) add checks for programs, files, libraries (checkProgram, checkFile, checkLibrary)
#  4) add custom-checks with addCheck(...)
#  5) include basic-configuration functionality with CFG.includeConfig - have a look at build/configs/unix.rb for more information on this
#  6) run the configuration with CFG.run - a config.rb file will be written 
#

require 'build/platform.rb'
require 'build/config_tools.rb'

alias :oldGetConfig :getConfig
def getConfig(n)
	v=CFG.get(n)
	v=oldGetConfig(n) if v.nil?
	#puts "GETCONFIG #{n}:#{v}"
	v
end

# the CFG-module serves with many functions that may be used in a configure script
module CFG
	@@config={}
	@@checks=[]
	@@options={}
	@@failed=[]

	def self.setOptions(ops)
		ops.each{|k,v|CFG.set(k,v)}
	end

	# return an array containing a hash, that describes, how the possible options will be checked
	# have a loog at CFG.addOption
	def CFG.options
		@@options
	end

	# add check identified by *name*
	def CFG.addCheck(name,needed=true,&proc)
		@@checks << {:name=>name,:proc=>proc,:needed=>needed}
	end

	def CFG.run
		checkCompile
		ok=runChecks
		saveConfig if ok
	end

	def CFG.runCheck(name,needed=true,&proc)
		text="checking "+name+" ..."
		print text
		ok=proc.call
		#ok=(not ok.nil?)
		print " "*(40-text.length)
		puts ({true=>"ok",false=>"failed",nil=>"failed"}[ok])
		@@failed << c[:name] unless ok or not c[:needed]
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
			puts ({true=>"ok",false=>"failed",nil=>"failed"}[ok])
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
#{@@config.collect{|k,v|"  '#{k}'=>"+toS(v)}.join(",\n")}
}
EOT
		f.close
		#puts File.open(configName).read
	
	end

	def CFG.set(n,v)
		@@config[n]=v
	end
	def CFG.get(n)
		@@config[n]
	end

	def CFG.checkProgram(program,needed=true)
# 		addOption("path-"+program,"",
# 			"set path to program '#{program}' like this:\n --path-#{program}=/usr/local/bin/#{program}","path") do |d|
# 				set(program,d)
# 			end

		addCheck("program "+program,needed) do
			path=get(program)
			path||=findProgram(program)
			r=testProgram(path)
			#puts "found at #{path}"
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

	# check if +path+ exists. Maybe we should check, if +path+ is executable
	def CFG.testProgram(path)
		#puts path,File.exists?(path),"--"
			File.exists?(path)
	end

	def CFG.includeConfig
		avail=["unix","mingw32"]
		addCheck ("base-config") do ||
			c=get("base-config")
			c=nil unless avail.member?(c)
			c||="unix"
			require "build/configs/"+c+".rb"
			$config.each{|k,v|@@config[k]||=v}
			true
		end
	end

	# *FIXME: implement this*
	# This function will test if the compiler is able to compile.
	def CFG.checkCompile
		
	end


	def CFG.gatherMkmfInfo
		CFG.set("RUBYLIB",getConfig("LIBRUBYARG_SHARED"))
	end

	def CFG.checkLibrary(libname,description,needed=true)
		addCheck("lib"+libname,needed) do ||
			#puts "FIXME checkLibrary #{libname}  #{description}"
			if true
				
				# C-check
				begin Dir.mkdir(".tmp"); rescue ;end
				filename=".tmp/test.c"
				f=File.open(filename,"w")
				f.puts "int main(){return 0;}"
				f.close
				require 'build/build.rb'
				Build.silent=true
				Build.compile(filename)
				obj=Build.cFileToObj(filename)
				exe=".tmp/test"
				Build.link(exe,[obj],["-l"+libname])
				result=system(exe)
				
				set("LIBS",get("LIBS").to_s+" -l#{libname}") if result
				result
			end
			

		end
	end

	def self.hasLibrary(libname)
		get("LIBS").split(" ").map{|l|l[2..-1]}.member?(libname)
	end


	private

	# this function parses the cli-inputs
	#
	# *FIXME: replace this with the standard ruby-cli-input-parser
	def CFG.parseArgsOld
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
						@@options.each{|op|
							if op[:short]==byte
								puts "* #{op[:help]}"
								op[:proc].call(true)
							end
						}
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
						puts "* #{op[:help]}"
						op[:proc].call(found)
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

	# convert *v* into a string. used by CFG.saveConfig to write the config.rb
	def CFG.toS(v)
		case v
			when String
				"'"+v.to_s+"'"
			when Array
				"'"+v.join(";")+"'"
			when Hash
				"'"+v.collect{|k,v|"#{k}=>#{v}"}.join(";")+"'"
			else
				#puts v,v.class
				#raise "MUH"
				v.to_s
		end
	end
end


def getConfigDirect(v,config={})
	CFG.get(v)
end