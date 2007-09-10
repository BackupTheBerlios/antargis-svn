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

# really nothing special here - just a "grouper"
class AntBaseTree<AntRubyEntity
	def initialize(p)
		super
		#setType("tree")
		setProvide("tree",true)
	end
	def resourceChanged
		super
		setupMesh
	end
end

# a fir
class AntFir<AntBaseTree
	def initialize()
		super(AGVector2.new(0,0))
		setProvide("fir",true)
		setProvide("wood",true)
		resource.set("wood",25)

		setupMesh
	end

	def resourceChanged
		if resource.get("wood")<=0
			@typeID=-1
			setProvide("wood",false)
		end
		super
	end

	
protected
	def setupMesh
		#if resource.get("wood")<=0
		#	data=getMeshData("data/models/stub.ant2",0.04,"data/textures/models/stub.png")
# 		setMesh(Mesh.new(getMap.getScene,meshData,AGVector4.new,0))
# 	end
# private
# 	def meshData
		typeId="stub"
		if resource.get("wood")>0
			typeId=1
# 			data=getMeshData("data/models/stub.ant2",0.04,"data/textures/models/stub.png")
# 		else
# 			data=getMeshData("data/models/fir2.ant2",0.45,"data/textures/models/fir7.png")
		end
		setMesh(typeId,:tree)
		#data.setTransparent(true)
		#data.setCulling(false) # patch for old trees

# 		return data
	end
end

# a birch representing a tree without fruit
class AntBirch<AntBaseTree
	def initialize()
		super(AGVector2.new(0,0))
	
		setupMesh
	end
	
protected
	def setupMesh
		if MyAntargislib.opengl
			setMesh(makeBirchTreeMesh)
		else
			puts "NO BIRCHES WITHOUT GL ATM"
		end
	end
end
