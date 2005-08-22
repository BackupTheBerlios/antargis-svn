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