#!/usr/bin/env ruby
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

#!/usr/bin/ruby

# some very simple base-class for animals. contains really nothing.
class AntAnimal<AntRubyEntity
	def AntAnimal.xmlName
		""
	end
end

class AntSheep<AntAnimal
	def initialize()
		super(AGVector2.new(0,0))
		setProvide("sheep",true)
		setSpeed 0.4
		@lastBirth=0
		@foodAdd=0
		
		data=getAnimMeshData("data/models/sheep.anim")
		setMesh(AnimMesh.new(getMap.getScene,data))

		resource.set("food",1)
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
			puts "A SHEEP IS BORN"
			sheep=AntSheep.new
			sheep.setPos(getPos2D)
			getMap.insertEntity(sheep)
			#getMap.endChange
			newRestJob(2)
			@lastBirth=-rand()*10
		elsif rand<0.5 then
			newMoveJob(0,getTargetPos,0)
			setMeshState("go")
		else
			newRestJob(3)
			setMeshState("eat")
			playSound("sheep")
		end

		@foodAdd+=1
		#puts "FOOOOOD #{@foodAdd}"
		if @foodAdd>3 and resource.get("food")<10
			#puts "RESOURCE: #{resource.get("food")}"
			resource.add("food",1)
			#puts "RESOURCE: #{resource.get("food")}"
			@foodAdd=0
		end

		@lastBirth+=1
	end
	
	def getTargetPos
		p=getPos2D
		tries=10
		# assure that sheep doesn't walk into water
		while tries>0 do
			d=AGVector2.new(rand-0.5,rand-0.5).normalized*2
			t=p+d
			t=getMap.truncPos(t)
			tries-=1
			if getMap.getHeight(t.x,t.y)>0
				return t
			end
		end 
		return p
	end
	def setMeshState(s)
		getFirstMesh.setAnimation(s)
	end
	def getName
		return "Corpse" if @dead
		"Sheep"
	end
	def eventDie
		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/rip.ant2",0.3,"data/textures/models/rip.png"),AGVector4.new(0,0,0,0),0))
		setProvide("sheep",false)
		@dead=true
		newRestJob(1)
	end
end
