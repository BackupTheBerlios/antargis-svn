#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_sheep.rb
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


class AntNewSheep<AntMyEntity
	def initialize()
		super(Pos2D.new(0,0))
		setType("sheep")
		setSpeed 40
		@lastBirth=0
	end
	def getSurfaceName
		return "sheep1"
	end
	def getTexture
		return "sheep"+mDirNum.to_s
	end
	def getVirtualY
		40
	end
	def saveXML(node)
		super(node)
		#node.set("typeID",@typeID.to_s)
	end
	def loadXML(node)
		super(node)
		#@typeID=node.get("typeID").to_i
	end
	def xmlName
		return "antNewSheep"
	end
	def noJob
		jobFinished
	end
	def jobFinished
		super
		# BIRTHRATE is here:
		if @lastBirth>40 then
			# make child
			puts "A SHEEP IS BORN"
			sheep=AntNewSheep.new
			sheep.setPos2D(getPos2D)
			$map.insertEntity(sheep)
			getMap.endChange
			newRestJob(2)
			@lastBirth=-rand()*10
		elsif rand<0.5 then
			newMoveJob(0,getTargetPos,0)
		else
			newRestJob(1)
		end
		@lastBirth+=1
	end
	
	def getTargetPos
		puts "GettargetPos"
		p=getPos2D
		t=p
		ok=true
		# assure that sheep doesn't walk into water
		while $map.getHeight(t)<15 or ok do	
			d=Pos2D.new(rand*31-15,rand*31-15).normalized
			d=d*50
			t=p+d
			puts "HUPE"
			puts t
			puts $map.getHeight(t)
			ok=false
		end 
		return t
	end
end
