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

#require 'fir_mesh.rb'
#require 'ant_appletree.rb'
require 'ant_grass.rb'


# def getTreeStub
# 	getMeshData("data/models/stub.ant2",0.04,"data/textures/models/stub.png")
# end
# 
# def getTreeTypes
# 	files=[
# 		getMeshData("data/models/fir2.ant2",0.45,"data/textures/models/fir5.png"),
# 		getMeshData("data/models/fir2.ant2",0.45,"data/textures/models/fir7.png"),
# #		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/fir5.png"),
# 		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree3.png"),
# 		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree5.png"),
# 		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree9.png"),
# 		getMeshData("data/models/tree6.ant2",0.45,"data/textures/models/tree5.png"),
# 		#getMeshData("data/models/tree1.ant2",1,"data/textures/models/fir_complete.png"),
# 		#getMeshData("data/models/tree1.ant2",1,"data/textures/models/birch_complete.png"),
# 		getMeshData("data/models/tree5.ant2",0.45,"data/textures/models/tree10.png"),
# 	]
# 	files.each{|f|f.setCulling(false)} # patch for old trees
# 	files+=[
# 		getMeshData("data/models/tree_simple1.ant2",0.3,"data/textures/models/tree_simple1.png"),
# 		getMeshData("data/models/tree_simple2.ant2",0.3,"data/textures/models/tree_simple1.png"),
# 		getMeshData("data/models/tree_simple5.ant2",0.3,"data/textures/models/tree_simple5.png")
# 	]
# 
# end
# 
# def getTreeMeshByType(type)
# 	if type<0
# 		return getTreeStub
# 	end
# 	d=getTreeTypes[type]
# 	d||=getTreeTypes[0]
# 	
# 	d.setTransparent(true)
# 	#d.setCulling(true) #false)
# 	return d
# end

class AntTree<AntRubyEntity
	def initialize(typeID=nil)
		super(AGVector2.new(0,0))
		@typeID=typeID
		#@typeID||=(getRand*getTreeTypes.length).to_i
		@typeID||=(getRand*(getMeshCount(:tree)-1)).to_i
		setProvide("wood",true)
		setProvide("fruit",true)
		@angle=getRand*360
		
		@maxApples=100
		setMinimapColor(AGColor.new(0,0x77,0))
	
		resource.set("fruit",5)
		resource.set("wood",20)
		resourceChanged
	end
	def resourceChanged
		if resource.get("wood")<=0
			@typeID=-1
			setProvide("fruit",false)
			setProvide("wood",false)
		end
		setupMesh
	end
	
	def setTreeType(t)
		@typeID=t
	end
	def saveXML(node)
		super(node)
		node.set("typeID",@typeID.to_s)
	end
	def loadXML(node)
		super(node)
		@typeID=node.get("typeID").to_i
		setupMesh
	end
	
	def eventNoJob
		# grow apples
		resource.set("fruit",[resource.get("fruit")+1.7,@maxApples].min)
		resourceChanged
		newRestJob(20)
	end
	
	private
	def setupMesh
		typeId="stub"
		if @typeID>=0
			typeId=@typeID%10
		end
		setMesh(createModel(:tree,typeId,@angle))
	end

	# an old function for display apples - this is too costly
	def setupApples
		@crownMiddle=AGVector3.new(0,0,3)
		@crownRadius=1.3
		@applePos=[]
		for i in 1..@maxApples
			va=(getRand-0.5)*Math::PI
			ha=(getRand+1)*Math::PI  # only on front side
			z=@crownRadius*Math::sin(va)
			rest=Math::sqrt(@crownRadius**2-z**2)
			y=Math::sin(ha)*rest
			x=Math::cos(ha)*rest
			@applePos.push(AGVector3.new(x,y,z)+@crownMiddle)
		end
	end
end

