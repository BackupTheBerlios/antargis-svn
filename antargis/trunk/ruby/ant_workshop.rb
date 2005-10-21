#!/usr/bin/env ruby

class AntWorkshop<AntHouse
	def initialize
		super
		setType("workshop")
		@lastBirth=0
		mesh=Mesh.new(getMeshData("data/models/workshop.ant",1.8),AGVector4.new(0,0,0),-50)
#		mesh=Mesh.new(getMeshData("data/models/farm.ant",2,"data/textures/models/tower_tex.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end

	# what's needed most ATM?
	# returns: [good,from] or nil
	def needed()
		goods={"wood"=>"tree","stone"=>"stone","food"=>"farm"}
		min=20
		need=nil
		needfrom=nil
		goods.each{|good,from|
			v=resource.get(good)
			if min>v then
				min=v
				need=good
				needfrom=from
			end
		}
		if need==nil then
			return nil
		else
			return [need,needfrom]
		end
	end

	def xmlName
		"antWorkshop"
	end
end