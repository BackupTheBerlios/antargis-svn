#!/usr/bin/env ruby

class AntWorkshop<AntHouse
	def initialize
		super
		setProvide("workshop",true)
		@lastBirth=0
		setDirection(-50)
		@smoke=0
	end

	def incSmoke
		@smoke+=1
		checkSmoke
	end
	def decSmoke
		@smoke-=1
		checkSmoke
	end

	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/workshop.ant2",0.18,"data/textures/models/workshop.png"),AGVector4.new(0,0,0),-50)
		setMesh(mesh)
		p=AGVector3.new(-1.3,-1.2,2.2)
		addMesh(@smokeMesh=Smoke.new(getMap.getScene,5),p)
		checkSmoke
		#addMesh(Mesh.new(getMap.getScene,getMeshData("data/models/apple.ant",0.03),AGVector4.new(0,0,0,0),0),p)
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
		goods=["wood","stone","food"]
		minarg=goods.min {|a,b|resource.get(a)<=>resource.get(b)}
		minval=resource.get(minarg)
		if minval>=10
			return nil
		end
		return minarg
	end

	def xmlName
		"antWorkshop"
	end

private
	def checkSmoke
		if @smokeMesh
			if @smoke
				@smokeMesh.setEnabled((@smoke>0))
			else
				@smokeMesh.setEnabled(false)
			end
		end
	end
end