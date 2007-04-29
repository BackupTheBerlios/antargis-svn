#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_sheep.rb
# by David Kamphausen (david.kamphausen@web.de)
#
# The "Antargis" project, including all files needed to compile it,
# is free software; you can redistribute it and/or use it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 2 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# You should have received a copy of the GNU General Public
# License along with this program.
#

def getFishMeshData
	getMeshData("data/models/fish.ant2",0.02,"data/textures/models/fish.png")
end

class AntFish<AntAnimal
	def initialize()
		super(AGVector2.new(0,0))
		setProvide("fish",true)
		setSpeed 0.6
		@lastBirth=0
		@foodAdd=0

		setOnGround(false)
		setPos(AGVector3.new(0,0,-0.3))

		puts "FISH:#{getPos3D}"
		
		setMesh(createModel(:fish))
		#mesh=Mesh.new(getMap.getScene,getFishMeshData,AGVector4.new(0,0,0),0)
		#setMesh(mesh)

		resource.set("food",1)
		puts "FISH:#{getPos3D}"
	end
	def saveXML(node)
		super(node)
	end
	def loadXML(node)
		super(node)
	end
	def eventNoJob
		eventJobFinished
	end
	def eventJobFinished
		super

		if @dead
			newRestJob(30)
			if @alreadyDead
				getMap.removeEntity(self)
			end
			@alreadyDead=true
	
			return
		end

		# BIRTHRATE is here:
		if @lastBirth>40 then
			# make child
			puts "A FISH IS BORN"
			fish=AntFish.new
			fish.setPos(getPos2D)
			getMap.insertEntity(fish)
			getMap.endChange
			newRestJob(2)
			@lastBirth=-getRand()*10
		else
			setPos(AGVector3.new(getPos2D,-0.3))
			newMoveJob(0,getTargetPos,0)
			setMeshState("go")
		end

		@foodAdd+=1
		#puts "FOOOOOD #{@foodAdd}"
		if @foodAdd>3 and resource.get("food")<10
			resource.add("food",1)
			@foodAdd=0
		end

		@lastBirth+=1
	end
	
	def getTargetPos
		p=getPos2D
		tries=10
		# assure that sheep doesn't walk into water
		while tries>0 do
			d=AGVector2.new(getRand-0.5,getRand-0.5).normalized*2
			t=p+d
			t=getMap.truncPos(t)
			tries-=1
			if getMap.getHeight(t.x,t.y)<0
				return t
			end
		end 
		return p
	end
	def setMeshState(s)
		#getFirstMesh.setAnimation(s)
	end
	def getName
		"Fish"
	end
end
