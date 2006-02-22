#!/usr/bin/env ruby

class AntWorkshop<AntHouse
	def initialize
		super
		setProvide("workshop",true)
		@lastBirth=0
		setDirection(-50)
	end
	def setupMesh
		mesh=Mesh.new(getMeshData("data/models/workshop.ant2",1.8,"data/textures/models/workshop.png"),AGVector4.new(0,0,0),-50)
#		mesh=Mesh.new(getMeshData("data/models/farm.ant",2,"data/textures/models/tower_tex.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
		p=AGVector3.new(-1.3,-1.2,2.2)
		addMesh(Smoke.new(5),p)
		addMesh(Mesh.new(getMeshData("data/models/apple.ant",0.03),AGVector4.new(0,0,0,0),0),p)
	end
	
	def assignJob(e)
		if e.getMode=="invent"
			produceOneTool
			e.setMode("any")
		end
		if not @job
			if allThereForTool && notEnoughTools && atHome(e)
				e.setMode("invent")
				e.newRestJob(10)
				startToolProduction
				return
			end
		end
		super(e)
	end
	
	def startToolProduction
		resource.sub("wood",1)
		resource.sub("stone",1)
	end
	
	def produceOneTool
		resource.add("tool",1)
	end
	
	def allThereForTool
		resource.get("wood")>=1 && resource.get("stone")>=1
	end
	
	def notEnoughTools
		resource.get("tool")<10
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