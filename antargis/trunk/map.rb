#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# map.rb
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

class AntRubyMap<AntargisMap
	def initialize(w,h)
		super(w,h)
		@ents={}
	end
	def loadEntity(node)
		e=super(node)
		if e then return e end
		if node.getName=="antNewMan" then
			e=AntNewMan.new
		end
		return e
	end
	# marshall, so that ruby-objects get used
	def insertEntity(e)
		super(e)
		@ents[e.getID]=e
	end
	def removeEntity(e)
		super(e)
		@ents.deleteAt(e.getID)
	end
	def getNext(me,t)
		e=super(me,t)
		if e==nil then return e end
		return @ents[e.getID]
	end
	def getEntity(id)
		return @ents[id]
	end
	
	def clear
		super
		@ents.clear
	end
		
end

def getMap
	$map
end