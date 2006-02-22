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

class AntBaseTree<AntMyEntity
end

class AntFir<AntBaseTree
	def initialize()
		super(AGVector2.new(0,0))
		setProvide("tree",true)
		setProvide("fir",true)
	
		setupMesh
	end
	def resourceChanged
		setupMesh
	end
	
	def xmlName
		return "antFir"
	end
	
	private
	def meshData
		data=getMeshData("data/models/fir2.ant2",0.45,"data/textures/models/fir7.png")
		data.setTransparent(true)
		return data
	end
	def setupMesh
		#setMesh(makeFirMesh)
		setMesh(Mesh.new(meshData,AGVector4.new,0))
	end
end

class AntBirch<AntMyEntity
	def initialize()
		super(AGVector2.new(0,0))
		setType("tree")
	
		setupMesh
	end
	def resourceChanged
		setupMesh
	end
	
	def xmlName
		return "antBirch"
	end
	
	private

	def setupMesh
		setMesh(makeBirchTreeMesh)
	end
end
