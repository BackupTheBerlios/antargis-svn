#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_tree.rb
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

require 'fir_mesh.rb'
require 'ant_appletree.rb'
require 'ant_grass.rb'


def getTreeTypes
	files=[
		getMeshData("data/models/fir2.ant2",0.45,"data/textures/models/fir5.png"),
		getMeshData("data/models/fir2.ant2",0.45,"data/textures/models/fir7.png"),
#		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/fir5.png"),
		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree3.png"),
		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree5.png"),
		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree9.png"),
		getMeshData("data/models/tree6.ant2",0.45,"data/textures/models/tree5.png"),
		getMeshData("data/models/tree1.ant2",1,"data/textures/models/fir_complete.png"),
		getMeshData("data/models/tree1.ant2",1,"data/textures/models/birch_complete.png"),
		getMeshData("data/models/stub.ant2",0.04,"data/textures/models/stub.png"),
		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree10.png"),
	]
end

def getTreeMeshByType(type)
	d=getTreeTypes[type]
	d||=getTreeTypes[0]
	d.setTransparent(true)
	return d
end

class AntNewTree<AntMyEntity
	def initialize(typeID=nil)
		super(AGVector2.new(0,0))
		@typeID=typeID
		@typeID||=(rand*getTreeTypes.length).to_i
		setProvide("tree",true)
		@angle=rand*360
		
		setMinimapColor(AGColor.new(0,0x77,0))
		#setVirtualY(100)
	
		@crownMiddle=AGVector3.new(0,0,3)
		@crownRadius=1.3
		@maxApples=100
		@applePos=[]
		for i in 1..@maxApples
			va=(rand-0.5)*Math::PI
			ha=(rand+1)*Math::PI  # only on front side
			z=@crownRadius*Math::sin(va)
			rest=Math::sqrt(@crownRadius**2-z**2)
			y=Math::sin(ha)*rest
			x=Math::cos(ha)*rest
			@applePos.push(AGVector3.new(x,y,z)+@crownMiddle)
		end
		resource.set("food",5)
		setupMesh
	end
	def resourceChanged
		setupMesh
	end
	
	def setTreeType(t)
		@typeID=t
	end
	def xmlName
		return "antNewTree"
	end
	def saveXML(node)
		super(node)
		node.set("typeID",@typeID.to_s)
	end
	def loadXML(node)
		super(node)
		@typeID=node.get("typeID").to_i
	end
	
	def eventNoJob
		# grow apples
		resource.set("food",[resource.get("food")+1,@maxApples].min)
		resourceChanged
		newRestJob(20)
	end
	
	private
	def setupMesh
		setMesh(Mesh.new(getMap.getScene,getTreeMeshByType(@typeID),AGVector4.new(0,0,0,0),0))
		return
	
		setMesh(makeAppleTreeMesh(@angle))
		
		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/stub.ant2",0.04,"data/textures/models/stub.png"),AGVector4.new(0,0,0,0),0))
		data=getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree3.png")
		data.setTransparent(true)
		setMesh(Mesh.new(getMap.getScene,data,AGVector4.new(0,0,0,0),0))
		
		
		
		return
		for i in 1..([@maxApples,resource.get("food")*3].min)
			p=@applePos[i-1]
			addMesh(Mesh.new(getMeshData("data/models/apple.ant",0.03),AGVector4.new(0,0,0,0),0),p)
		end
	end
end

