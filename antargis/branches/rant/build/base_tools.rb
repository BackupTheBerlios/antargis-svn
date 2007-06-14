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