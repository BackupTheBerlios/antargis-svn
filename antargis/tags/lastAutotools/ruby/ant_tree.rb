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

class AntNewTree<AntMyEntity
	def initialize()
		super(AGVector2.new(0,0))
		@typeID=(rand()*12).to_i
		setType("tree")
		@angle=rand*360
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
	def getTexture
		return "tree"+@typeID.to_s+".png"
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
		#makeFirMesh
		#setMesh(Mesh.new(getMeshData("data/models/bare_tree.ant2",1),AGVector4.new(0,0,0,0),0))
		#setMesh(GLTree.new(AGVector4.new(0,0,0,0),3))
		setMesh(makeAppleTreeMesh(@angle))
		#setMesh(makeGrassMesh(rand*0.25+0.25))
		return
		for i in 1..([@maxApples,resource.get("food")*3].min)
			p=@applePos[i-1]
			addMesh(Mesh.new(getMeshData("data/models/apple.ant",0.03),AGVector4.new(0,0,0,0),0),p)
		end
	end
end

