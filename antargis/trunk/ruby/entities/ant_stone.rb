#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_stone.rb
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

class AntStone<AntRubyEntity
	def initialize(map)
		super
		@typeID=(getRand*2).to_i
		setProvide("stone",true)
		#setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/big_stone.ant2",0.7,"data/textures/models/big_stone.png"),AGVector4.new(0,0,0,0),getRand*360))
		mesh=setMesh
		mesh.setRotation(getRand*360)
		resource.set("stone",500)
	end
	def setTreeType(t)
		@typeID=t
	end
	def getTexture
		return "deco"+@typeID.to_s+".png"
	end
	def saveXML(node)
		super(node)
		node.set("typeID",@typeID.to_s)
	end
	def loadXML(node)
		super(node)
		@typeID=node.get("typeID").to_i
	end
end

