require 'build/platform.rb'
require 'config.rb'                   # include build-options
require 'build/config_tools.rb'
require 'build/build.rb'

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




def U2W(path)
    path.gsub("/",Dir.separator)
end

def isWindows
	(not ENV['WINDIR'].nil?)
end

