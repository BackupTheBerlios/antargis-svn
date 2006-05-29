#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_formations.rb
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

class AntFormation<AntFormation
	def initialize(boss)
		@boss=boss
	end
	def getSitFormation(man)
	end
	def getWalkFormation(man,dir)
	end
	def getPosition(line,row)
	end
end

class AntFormationRest
	def getPosition(line,row)
		radius=line*1.5
		angle=row.to_f/getRowsOfLine(line)*Math::PI*2
		AGVector2.new(Math::cos(row)*radius,Math::sin(row)*radius)
	end

	def getRowsOfLine(line)
		{1=>8,2=>14,3=>20}[line]
	end
end

class AntFormationBlock<AntFormation
	def getPosition(line,row)
		
	end
end

# FIXME: not yet implemented
#
# class AntFormationV
# end
# 
# class AntFormationCircle
# end