require "ant_sound.rb"

def getMeshData(file,zoom,texture="",shadow=true)
	id=file+":"+texture
	if not $meshes
		$meshes={}
	end
	if not $meshes[id]
		puts "LOAD MESH:"+id	
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

module Libantargisbasic
	class AGVector3
		def x=(p)
			setX(p)
		end
		def y=(p)
			setY(p)
		end
		def z=(p)
			setZ(p)
		end
	end
	class AGVector2
		def _dump(depth)
			[x,y].pack("gg")
		end
		def AGVector2._load(s)
			a,b=s.unpack("gg")
			AGVector2.new(a,b)
		end
	end
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

def trace
	puts "TRACE #{caller[0]}"
end

def assert(&block)
	if not block.call
		raise "Assertion #{block} failed in #{caller[0]} #{block.binding}"
	end
end


