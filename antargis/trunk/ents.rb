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
		mRUBY=self
	end
end

require 'ant_hero.rb'
require 'ant_sheep.rb'
# MAN
class AntNewMan<AntMyEntity
	def initialize()
		super(Pos2D.new(0,0))
		setType("man")
		@signed=false
	end
	def getTexture
		return "man"+mDirNum.to_s
	end
	
	def noJob
		jobFinished
	end
	
	def setNoBoss()
		setVar("bossName","")
	end
	
	def setBoss(hero)
		setVar("bossName",hero.getName)
		hero.signUp(self)
	end
	
	#def gotNewJob
	#	puts "GOTNEWJOB:"+self.to_s
	#end
	
	def jobFinished
		super
		
		if getVar("bossName")=="" then
		
			house=getMap.getNext(self,"house")
			houseName=house.getVar("name")
			if houseName=="" then
				puts "ERROR House has no name!"
				exit
			end
			setVar("bossName",houseName)
			newRestJob(rand()*2)
			house=getMap.getRuby(house)
			house.signUp(self)
			@signed=true
		else
			boss=getMap.getByName(getVar("bossName"))
			if not @signed then
				puts getVar("bossName")
				boss.signUp(self)
				@signed=true
			end
			boss.assignJob(self)
		end
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
		node.set("bossName",getVar("bossName"))#@bossID.to_s)
	end
	def loadXML(node)
		super(node)
		setVar("bossName",node.get("bossName"))
	end
end


class AntNewTree<AntMyEntity
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


class AntNewStone<AntMyEntity
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
	def xmlName
		return "antNewStone"
	end
end

class AntNewHouse<AntEntity
	def initialize
		super(Pos2D.new(0,0))
		@type=2
		setType("house")
		@men=[]
		@atHome=[]
		@lastBirth=0
	end
	
	def signUp(man)
		puts "SIGNUP"
		puts man
		@men.push(man)
	end
	
	def removeMan(man)
		@men.delete(man)
		@atHome.delete(man)
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
	#def insertMan(m)
	#	@men.push(m)
	#end
	def xmlName
		return "antNewHouse"
	end
	
	def menCount
		@men.length
	end
	
	def takeMan
		m=@men[0]
		removeMan(m)
		return m
	end
	
	def checkBirth
		# only men, which are at home can add to birth rate
		@lastBirth+=@atHome.length
		puts "LASTBIRTH:"+@lastBirth.to_s
		if @lastBirth>50 then
			puts "A MAN IS BORN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
			man=AntNewMan.new
			man.setPos2D(getPos2D)
			getMap.insertEntity(man)
			getMap.endChange
			@lastBirth=-10*rand
		end
	end
	
	def noJob
		jobFinished
	end
	def jobFinished
		checkBirth
		newRestJob(2)
	end
	def assignJob(e)
		if atHome(e) then
			@atHome.push(e)
			# is home:
			# 1) take everything from inventory
			resource.takeAll(e.resource)
			# 2) give job
			need=needed()
			# keep at least a third of all men at home
			if need != nil and @atHome.length>@men.length/3 then
				fetch( need[1],need[0],e)
				@atHome.delete(e)
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
