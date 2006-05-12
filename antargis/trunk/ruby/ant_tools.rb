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