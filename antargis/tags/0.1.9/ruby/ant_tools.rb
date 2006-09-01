#!/usr/bin/env ruby

require "ant_sound.rb"

def getMeshData(file,zoom,texture="",shadow=true)
	id=file+":"+texture
	if not $meshes
		$meshes={}
	end
	if not $meshes[id]
		puts "LOAD MESH:"+id	
		$meshes[id]=MeshData.new(file,zoom,texture,shadow)

# 		# instancing test
# 		puts "FILE:#{file}"
# 		if file =~ /tree/
# 			ts=[]
# 			(0..2).each{|x|
# 				(0..2).each{|y|
# 					ts.push(AGMatrix4.new(AGVector4.new(x*2+rand*1.7,y*2+rand*1.7,0,0)))
# 				}
# 			}
# 			$meshes[id].makeInstances(ts)
# 		end
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

module Libantargis
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
