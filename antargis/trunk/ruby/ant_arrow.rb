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


# AntArrow is an entity class for displaing an arrow. That's all about it.
# FIXME: Maybe this should be moved to plain C++ in a kind of ParticleSystem with meshes.

class AntArrow<AntRubyEntity
	def initialize
		super(AGVector2.new(0,0))
		@typeID=(rand()*2).to_i
		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/arrow.ant2",0.1,"data/textures/models/arrow.png"),AGVector4.new(0,0,0,0),rand*360))
		setSpeed(10)
	end
	def eventNoJob
		getMap.removeEntity(self)
	end
end

