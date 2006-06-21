#!/usr/bin/env ruby


# This is a workshop. Residents will produce tools and fetch needed resources
# which are wood and stone. Apart from this they'll try to gather food so they won't starve
class AntWorkshop<AntHouse
	def initialize
		super
		setProvide("workshop",true)
		@lastBirth=0
		setDirection(-50)
		@smoke=0
	end

	# increase smoke by one
	def incSmoke
		@smoke+=1
		checkSmoke
	end
	def decSmoke
		@smoke-=1
		checkSmoke
	end

	# assign my men a job.
	# this is simple tool-production in case there is no defend-job
	# * e - entity (= one of my men)
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
protected
	
	# start tool production (=remove 1 wood and 1 stone)
	def startToolProduction
		resource.sub("wood",1)
		resource.sub("stone",1)
	end
	

	# produce one tool
	def produceOneTool
		resource.add("tool",1)
	end
	
	# check if there are enough resources to build a tool (wood>=1 && stone=>1)
	def allThereForTool
		resource.get("wood")>=1 && resource.get("stone")>=1
	end
	
	# check if there are enough tools (10)
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

	# returns my name "workshop"
	def houseType
		"workshop"
	end

	# sets up the mesh and adds a smoke-particle engine, which is disabled at first
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/workshop.ant2",0.18,"data/textures/models/workshop.png"),AGVector4.new(0,0,0),-50)
		setMesh(mesh)
		p=AGVector3.new(-1.3,-1.2,2.2)
		addMesh(@smokeMesh=Smoke.new(getMap.getScene,5),p)
		checkSmoke
	end
	

private
	# checks if smoke should be displayed
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