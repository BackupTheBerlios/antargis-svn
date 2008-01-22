require "ruby/ant_sound.rb"

def getMeshData(file,zoom,texture="",shadow=true)
	id=file+":"+texture
	if not $meshes
		$meshes={}
	end
	if not $meshes[id]
		pp file,zoom,texture,shadow
		raise "Texture-file not found!" unless (fileExists(texture) or texture=="")
		$meshes[id]=MeshData.new(file,zoom,texture,shadow)
	end
	return $meshes[id]
end

def getAnimMeshData(file)
	if not $meshes
		$meshes={}
	end
	if not $meshes[file]
		$meshes[file]=AnimMeshData.new(file)
		$meshes[file].setTransform(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))

	end
	return $meshes[file]
end

def displayError(error)
	getSoundManager.playWave("data/sound/error.wav")
	playSound(error)
	dputs "ERROR:"+error.to_s
end

class Set<Array
	def push(x)
		super
		sort!
		uniq!
	end
end

def getDescendantsOfClass(p)
	c=[]
	ObjectSpace.each_object(Class){|a|c.push(a) if a.ancestors.member?(p)}
	return c
end

class Class
	def descendants
		getDescendantsOfClass(self)
	end
end

def trace
	puts "TRACE #{caller[0]}"
end

class AntargisException<Exception
	attr_accessor :text
	
	def to_s
		super+":"+text.to_s
	end
end

def assert(text="",&block)
	if not block.call
		exception=AntargisException.new
		exception.text="#{text} #{block} failed in #{caller[0]} #{block.binding}"
		raise exception
	end
end


