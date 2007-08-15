#require 'mkmf'                        # mkmf holds information about the compiler-settings while compiling ruby (by the maintainer)

require 'build/platform.rb'
require 'config.rb'                   # include build-options

def getDir(path)
	# FIXME: check for windows
	path.split(Dir.separator)[0..-2].join("/")
end

def getDirUnix(path)
    path.split("/")[0..-2].join("/")
end


def makeLibName(dir)
    #puts "makeLibName #{dir}"
	"antargis"+dir.split("/")[-1]
end

# build a command out of templates in config.rb
def makeCommand(cmd,output,input)
	#cmd.sub("§OUTPUT§",output).sub("§INPUT§",input)
	extendCommand($config,cmd,{"OUTPUT"=>output,"INPUT"=>input})
end


def extendCommandLine(config,s)
  r=config[s]
  return r if r.nil?
  r.gsub(/\$\(([^\)]*)\)/) {|a|
		n=a[2..-2] # filter "$(xy..z)" to "xy..z"
		raise "Endless recursion in #{n}!" if n==s
		extendCommandLine(config,n)  # replace recursive values within this form: $(...)
	}
end

def extendCommand(config,name,map={})
	c=config.clone
	map.each{|k,v|
		c[k]=v
	}
	extendCommandLine(c,name)
end

# the getConfig function reads out compiler-information out of the mkmf-configuration variables - these are defined in a recursive manner
def getConfig(s)
	extendCommand(CONFIG,s)
end

def U2W(path)
    path.gsub("/",Dir.separator)
end

def isWindows
	(not ENV['WINDIR'].nil?)
end

module Cmd
	def Cmd.sys(cmd)
		puts cmd
		#as=`#{cmd}`
		
		#puts "-#{cmd}-",cmd.class
		res=system(cmd)
		#puts a
		raise 1 unless res
		
	end
end


module Build
	include Cmd

	def Build.includes
		includes=Dir.glob(File.join("ext","**","*.h")).collect{|f|f.sub(/\/[^\/]*$/,"")}.uniq+[getConfig("archdir")]
		includes.collect{|i|"-I#{i}"}.join(" ")
	end

	def Build.cflags
		includes+" "+getConfig("CFLAGS")+" "+$config["CFLAGS"]
	end

	def Build.compile(cFile)
		cObj=cFileToObj(cFile)
		cmd=makeCommand("CXX_CALL",cObj,cflags+" "+cFile)
		Cmd.sys cmd
	end
	
	def Build.linkToLib(target,objs,libs)
		cmd=makeCommand("LINK_SHARED",target,(objs+libs).join(" "))
		Cmd.sys cmd
	end

	def Build.cFileToObj(filename)
		filename.sub(/\.cc$/,".oo").sub(/\.c$/,".o")
	end

end

