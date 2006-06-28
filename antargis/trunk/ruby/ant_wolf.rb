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

class AntWolf<AntAnimal
	def initialize()
		super(AGVector2.new(0,0))
		setProvide("wolf",true)
		setSpeed 0.6
		@lastBirth=0
		@foodAdd=0
		
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/ant_wolf.ant2",1,"data/textures/models/ant_wolf.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
		# data=getAnimMeshData("data/models/sheep.anim")
		# 		setMesh(AnimMesh.new(getMap.getScene,data))

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
				$map.removeEntity(self)
			end
			@alreadyDead=true
	
			return
		end

		# BIRTHRATE is here:
		if @lastBirth>40 then
			# make child
			puts "A SHEEP IS BORN"
			sheep=AntWolf.new
			sheep.setPos(getPos2D)
			$map.insertEntity(sheep)
			getMap.endChange
			newRestJob(2)
			@lastBirth=-rand()*10
		elsif rand<0.5 then
			newMoveJob(0,getTargetPos,0)
			setMeshState("go")
		else
			newRestJob(3)
			setMeshState("eat")
			playSound("wolf")
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
		puts "GettargetPos"
		p=getPos2D
		t=p
		ok=true
		# assure that sheep doesn't walk into water
		while $map.getHeight(t.x,t.y)<0.1 or ok do
			d=AGVector2.new(rand-0.5,rand-0.5).normalized*4
			t=p+d
			t=$map.truncPos(t)
			puts "HUPE"
			puts t
			
			puts $map.getHeight(t.x,t.y)
			ok=false
		end 
		return t
	end
	def setMeshState(s)
		getFirstMesh.setAnimation(s)
	end
	def getName
		return "Corpse" if @dead
		"Wolf"
	end
	def eventDie
		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/rip.ant2",0.3,"data/textures/models/rip.png"),AGVector4.new(0,0,0,0),0))
		@dead=true
		newRestJob(1)
	end
end
