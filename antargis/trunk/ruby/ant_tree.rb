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

class AntNewTree<AntMyEntity
	def initialize()
		super(AGVector2.new(0,0))
		@typeID=(rand()*12).to_i
		setType("tree")
		#setVirtualY(100)
		setMesh(Mesh.new(getMeshData("data/models/tree.ant",1),AGVector4.new(0,0,0,0),0))
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
end

