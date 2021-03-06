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


class AntMine<AntMyEntity
	def initialize(typeID=nil)
		super(AGVector2.new(0,0))
		setProvide("coal",true)
		setProvide("ore",true)
		@angle=rand*360
		
		setMinimapColor(AGColor.new(0,0,0))
	
		resource.set("ore",500)
		resource.set("coal",500)
		setupMesh
	end
	def resourceChanged
		setupMesh
	end
	
	def xmlName
		return "antMine"
	end
	
	private
	def setupMesh
		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/mine2.ant2",0.2,"data/textures/models/mine.png"),AGVector4.new(0,0,0),-40))
	end
end

