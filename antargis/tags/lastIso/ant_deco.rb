#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_deco.rb
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

class AntNewDeco<AntMyEntity
	def initialize(decoType=nil)
		super(Pos2D.new(0,0))
		@decoType=decoType
		if @decoType==nil
			a=["flower1a","flower2a","twig","gravel","small_bush","hole","grass4","grass4a","grass4b","gravel_big","gravel_big2","rock1"]
			r=(rand()*a.size).to_i
			@decoType=a[r]
		else
			b={"coach"=>["coach2"],
					"flower"=>["flower1a","flower2a"],
					"gravel"=>["gravel","gravel_big","gravel_big2"],
					"hole"=>["hole"],
					"twig"=>["twig"],
					"rock"=>["rock1"],
					"grassLight"=>["grass4a","grass4b","grass4"],
					"floor"=>["floor2"],
					"path"=>["path"],
					"block"=>["block2"],
					"grassGreen"=>["grass3"]}
			if ["grassGreen","grassLight","gravel","twig","hole","floor","path"].member?(@decoType) then
				setVirtualY(0)
			else
				setVirtualY(80)
			end
			a=b[@decoType]
			r=(rand()*a.size).to_i
			@decoType=a[r]
		end
		setType("deco")
	end
	def setDecoType(t)
		@typeID=t
	end
	def getTexture
		@decoType+".png"
	end
	def xmlName
		return "antNewDeco"
	end
	def saveXML(node)
		super(node)
		node.set("decoType",@decoType)
	end
	def loadXML(node)
		super(node)
		@decoType=node.get("decoType")
		#@typeID=node.get("typeID").to_i
	end
end

