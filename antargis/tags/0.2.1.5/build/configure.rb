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
			print " "*(60-text.length)
			puts ({true=>"ok",false=>"failed",nil=>"failed"}[ok])
			failed << c[:name] unless ok or not c[:needed]
		}
		if failed.length>0
			puts 
			puts "ERROR:"
			puts "The following tests failed:"
			failed.each{|f|puts f}
			puts ""
			puts "For more information look into the log-file config.log."
			puts "And don't hesitate to ask questions our forum: http://antargis.berlios.de/phpBB2"
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
	end

	def self.add(n,v)
		set(n,get(n).to_s+" "+v)
	end

	def CFG.set(n,v)
		@@config[n]=v
	end
	def CFG.get(n)
		puts "GET #{n}:#{@@config[n]}"


		@@config[n]
	end

	def CFG.checkProgram(name,program,needed=true)
		addCheck("program "+program,needed) do
			path=get(program)
			path||=findProgram(program)
			r=testProgram(path)
			#set(program,path) if r
			set(name,path) if r
			#pp @@config
			r
		end
	end

	def CFG.getPath
		p=ENV['PATH']
		psep={"/"=>":","\\"=>";"}[Dir.separator]
		#puts p,p.class,psep
		ps=p.split(psep)
		ps+=get("PATHS") if get("PATHS")

		#puts get("prefix")
		ps << get("prefix")+Dir.separator+"bin" if get("prefix")
		if Dir.separator=="\\"
				ps << (Dir.pwd+"/build/win32/usr/bin").gsub("/",Dir.separator)
		end
		ps
	end

	def CFG.findProgram(program)
		paths=getPath
		paths.each{|p|
			currentPath=p+Dir.separator+program
			if File.exists?(currentPath)
				return currentPath
			end
		}
		return findProgram(program+".exe") if Dir.separator=="\\" and not program=~/exe$/
	end

	# check if +path+ exists. Maybe we should check, if +path+ is executable
	def CFG.testProgram(path)
		File.exists?(path)
	end

	def CFG.includeConfig
		readConfig("base")



# 		avail=["unix","mingw32"]
# 		addCheck ("base-config") do ||
# 			c=get("base-config")
# 			c=nil unless avail.member?(c)
# 			c||="unix"
# 			require "build/configs/"+c+".rb"
# 			$config.each{|k,v|@@config[k]||=v}
# 			true
# 		end
	end

	# *FIXME: implement this*
	# This function will test if the compiler is able to compile.
	def CFG.checkCompile
		
	end

	def CFG.readConfig(file)
		require "build/configs/"+file+".rb"
		$config.each{|k,v|@@config[k]||=v}
	end


	def CFG.gatherMkmfInfo
		CFG.set("RUBYLIB",getConfig("LIBRUBYARG_SHARED"))
		CFG.set("LIBS",CFG.get("LIBS").to_s+" "+CFG.get("RUBYLIB"))
		#pp CFG.get("LIBS")
	end

	def CFG.checkCProgram(text,addlibs=[])
		require 'build/build.rb'
		begin Dir.mkdir(".tmp"); rescue ;end
		filename=".tmp/test.c"
		obj=Build.cFileToObj(filename)
		exe=".tmp/test"
		[filename,obj,exe].each{|f|begin File.delete(f); rescue;end}

		f=File.open(filename,"w")
		f.puts text
		f.close
		Build.log="config.log"
		Build.silent=true
		Build.compile(filename)
		Build.link(exe,[obj],addlibs)
		result=system(exe)
		#puts exe,result
		result
	end

	def CFG.checkLibrary(libname,description,needed=true)
		addCheck("lib"+libname,needed) do ||
			result=checkCProgram("int main(){return 0;}",["-l"+libname])
			set("LIBS",get("LIBS").to_s+" -l#{libname}") if result
			result
		end
	end

	def self.hasLibrary(libname)
		get("LIBS").split(" ").map{|l|l[2..-1]}.member?(libname)
	end

	def self.checkFunction(name,needed=true)
		addCheck("function #{name}",needed) do ||
			#puts CFG.get("LIBS")
			result=checkCProgram("extern \"C\" int #{name}(...);int main(){#{name}();return 0;}",[get("LIBS")])
			raise 1 unless result
			result
		end
	end

	def self.checkHeader(header,function=nil,error=nil,needed=true)
		error="header #{header}" if error.nil?
		addCheck(error,needed) do ||
			#function+="();" if function
			result=checkCProgram("#include <#{header}>\nint main(){void *f=(void*)#{function};return !f;}",[get("LIBS")])
		end
	end


	private


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
				v.to_s
		end
	end
end


def getConfigDirect(v,config={})
	CFG.get(v)
end