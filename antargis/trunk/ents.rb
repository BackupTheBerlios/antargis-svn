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


# MAN
class AntNewMan<AntEntity
	def initialize()
		super(Pos2D.new(0,0))
		setType("man")
	end
	def getTexture
		return "man"+mDirNum.to_s
	end
	def move(time)
		if not hasJob then
			if getVar("bossID")=="" then
				house=getMap.getNext(self,"house")
				setVar("bossID",house.getID.to_s)
				newRestJob(rand()*10)
			else
				getMap.getEntity(getVar("bossID").to_i).assignJob(self)
			end
		end
		super(time)
	end
	def getSurfaceName
		return "man1"
	end
	def xmlName
		return "antNewMan"
	end
	
	def saveXML(node)
		super(node)
		puts "SAVEXML"
		puts node.methods
		node.set("bossID",@bossID.to_s)
	end
	def loadXML(node)
		super(node)
		setVar("bossID",node.get("bossID"))
	end
end

class AntNewHero<AntEntity
	def initialize
		super(Pos2D.new(0,0))
		@men={}
		#@resources={}
		setType("hero")
	end
	def getSurfaceName
		return "hero1dl"
	end
	def xmlName
		return "antNewHero"
	end
	def saveXML(node)
		super(node)
	end
	def loadXML(node)
		super(node)
	end
	
	def assignJob(e)
	end
	# formation:
	# 1) wait for 3/4 of people are in formation but max. 5 seconds or so
	# 2) start all at once
end

class AntNewTree<AntEntity
	def initialize()
		super(Pos2D.new(0,0))
		@typeID=0
		setType("tree")
	end
	def setTreeType(t)
		@typeID=t
	end
	def getSurfaceName
		return "tree"+@typeID.to_s
	end
	def getVirtualY
		100
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

class AntNewStone<AntEntity
	def initialize()
		super(Pos2D.new(0,0))
		@typeID=0
		setType("stone")
	end
	def setTreeType(t)
		@typeID=t
	end
	def getSurfaceName
		return "deco"+@typeID.to_s
	end
	def getVirtualY
		40
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

class AntNewHouse<AntEntity
	def initialize
		super(Pos2D.new(0,0))
		@type=2
		setType("house")
		@men=[]
	end
	def setHouseType(t)
		@type=t
	end
	def getSurfaceName
		return "tower"+@type.to_s
	end
	def getVirtualY
		100
	end
	def insertMan(m)
		@men.push(m)
	end
	def move(t)
	end
	def xmlName
		return "antNewHouse"
	end
	def assignJob(e)
		if atHome(e) then
			# is home:
			# 1) take everything from inventory
			resource.takeAll(e.resource)
			# 2) give job
			need=needed()
			if need != nil then
				fetch( need[1],need[0],e)
			else
				e.newRestJob(10)
			end
		else
			# is anywhere - come home
			e.newMoveJob(0,getPos2D,0)#,false)
		end
	end
	
	# says if entity is at home
	def atHome(entity)
		pd=entity.getPos2D-getPos2D
		n=pd.norm2
		return n<30
	end
	
	# what's needed most ATM?
	# returns: [good,from] or nil
	def needed()
		goods={"wood"=>"tree","stone"=>"stone"}
		min=10000
		need=nil
		needfrom=nil
		goods.each{|good,from|
			v=resource.get(good)
			if min>v then
				min=v
				need=good
				needfrom=from
			end
		}
		if need==nil then
			return nil
		else
			return [need,needfrom]
		end
	end
	
	# assigns ent a job for fetching good from a enttype
	def fetch(enttype,good,ent)
		tent=getMap.getNext(self,enttype)
		if tent == nil then
			puts "No '"+good+"' found!"
		else
			ent.newFetchJob(0,tent.getPos2D,good)
		end
	end
end
