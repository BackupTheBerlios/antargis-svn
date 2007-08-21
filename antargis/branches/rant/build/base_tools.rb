require 'build/platform.rb'
require 'config.rb'                   # include build-options
require 'build/config_tools.rb'

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

def makeDepName(output)
	dep=".deps/"+output.gsub("/","_")
end

def getDependencies(output)
	dep=makeDepName(output)
	if File.exists?(dep)
		content=File.open(dep).read
		content=content.gsub(/^[^:]*:/,"")
		files=content.gsub("\\\n","").split(" ")
		#puts files
		return files
	end
	#raise 1
	[]
end

# build a command out of templates in config.rb
def makeCommand(cmd,output,input)
	#cmd.sub("§OUTPUT§",output).sub("§INPUT§",input)
	begin
		Dir.mkdir(".deps")
	rescue
	end
	dep=makeDepName(output)
	extendCommand($config,cmd,{"OUTPUT"=>output,"INPUT"=>input,"DEP"=>dep})
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

