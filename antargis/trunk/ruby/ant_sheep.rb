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

class AntNewSheep<AntMyEntity
	def initialize()
		super(AGVector2.new(0,0))
		setType("sheep")
		setSpeed 0.4
		@lastBirth=0
		#setMesh(Mesh.new(getMeshData("data/models/sheep.ant2",3,"data/textures/models/ant_sheep.png"),AGVector4.new(0,0,0,0),90))
		
		
		#setMesh(Mesh.new(getMeshData("data/models/sheep.ant2",3),AGVector4.new(0,0,0,0),90))
		
		
		data=AnimMeshData.new("data/models/sheep.anim")
		#data=AnimMeshData.new("data/models/man_fight.anim")
		data.setTransform(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))
		setMesh(AnimMesh.new(data))

		
	end
	def saveXML(node)
		super(node)
		#node.set("typeID",@typeID.to_s)
	end
	def loadXML(node)
		super(node)
		#@typeID=node.get("typeID").to_i
	end
	def xmlName
		return "antNewSheep"
	end
	def eventNoJob
		eventJobFinished
	end
	def eventJobFinished
		super
		# BIRTHRATE is here:
		if @lastBirth>40 then
			# make child
			puts "A SHEEP IS BORN"
			sheep=AntNewSheep.new
			sheep.setPos(getPos2D)
			$map.insertEntity(sheep)
			getMap.endChange
			newRestJob(2)
			@lastBirth=-rand()*10
		elsif rand<0.5 then
			newMoveJob(0,getTargetPos,0)
			getFirstMesh.setAnimation("go")
		else
			newRestJob(3)
			getFirstMesh.setAnimation("eat")
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
end
