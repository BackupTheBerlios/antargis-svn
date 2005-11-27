#!/usr/bin/env ruby

def getMeshData(file,zoom,texture="",shadow=true)
	if not $meshes
		$meshes={}
	end
	if not $meshes[file]
		$meshes[file]=MeshData.new(file,zoom,texture,shadow)
	end
	return $meshes[file]
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