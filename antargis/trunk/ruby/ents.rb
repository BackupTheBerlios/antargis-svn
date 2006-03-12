#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ents.rb
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

# try to implement alle entites in ruby
# WARNING: DON'T MEMBER_VARIABLES AS IT SEEMS TO CRASH RUBY SOMEHOW
# could be that it has something to do with Init_Stack ???


class AntMyEntity<AntEntity
	def initialize(p)
		super(p)
		$map.registerEntity(self)
		mRUBY=self
		@xmlProps={}
	end
	def setXMLProp(n,v)
		@xmlProps[n]=v
	end
	def getXMLProp(n)
		if @xmlProps[n]==nil
			return ""
		else
			return @xmlProps[n]
		end
	end
	def saveXML(node)
		super(node)
		@xmlProps.each{|n,v|
			node.set(n,v)
		}
	end
	def getDescription
		"This an entity - no more info here."
	end
	def <=>(e)
		to_s<=>e.to_s
	end
	def menCount
		0
	end
	def getPlayer
		nil
	end
end


require 'ant_hero.rb'
require 'ant_sheep.rb'
require 'ant_man.rb'
require 'ant_house.rb'
require 'ant_stone.rb'
require 'ant_tree.rb'
require 'ant_deco.rb'
require 'ant_tower.rb'
require 'ant_farmstead.rb'
require 'ant_farm.rb'
require 'ant_workshop.rb'
require 'ant_fir.rb'
require 'ant_grass.rb'
require 'ant_druid.rb'
require 'ant_decal.rb'
