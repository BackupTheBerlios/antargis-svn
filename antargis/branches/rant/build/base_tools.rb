require 'mkmf'                        # mkmf holds information about the compiler-settings while compiling ruby (by the maintainer)

class Dir
	def Dir.separator
		"/" # FIXME
	end
end

def getDir(path)
	# FIXME: check for windows
	path.split(Dir.separator)[0..-2].join("/")
end


def makeLibName(dir)
	"antargis"+dir.split(Dir.separator)[-1]
end

# the getConfig function reads out compiler-information out of the mkmf-configuration variables - these are defined in a recursive manner
def getConfig(s)
  r=CONFIG[s]
  return r if r.nil?
  r.gsub(/\$\(([^\)]*)\)/) {|a|getConfig(a[2..-2])} # replace recursive values within this form: $(...)
end

# build a command out of templates in config.rb
def makeCommand(cmd,output,input)
	cmd.sub("§OUTPUT§",output).sub("§INPUT§",input)
end