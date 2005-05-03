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

require 'libantargisruby'
require 'libantargis'
require 'antApp.rb'
#require 'sdl'

include Libantargisruby
include Libantargis


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
		#puts self
		#puts @bla
		if isJobFinished then
#			GC.start
			jobWasFinished
		end
		if not hasJob then
			if getVar("bossID")=="" then
				house=getMap.getNext(self,"house")
				setVar("bossID",house.getID.to_s)
				#house.insertMan(self)
				setJob(RestJob.new(rand()*10))
			else
				setJob(getMap.getEntity(getVar("bossID").to_i).getJob(self))
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
	def setFetchTarget(target,what)
		setVar("fetchWhat",what)
		#@fetchTarget=target
		#@fetchWhat=what
	end
	def jobWasFinished
		fetchWhat=getVar("fetchWhat")
		if fetchWhat!="" then
			resource.add(fetchWhat,1)
		end
		setFetchTarget("","")
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
	
	
	# formation:
	# 1) wait for 3/4 of people are in formation but max. 5 seconds or so
	# 2) start all at once
end

class AntNewTree<AntEntity
	def initialize()
		super(Pos2D.new(0,0))
		@type=0
		setType("tree")
	end
	def setTreeType(t)
		@type=t
	end
	def getSurfaceName
		return "tree"+@type.to_s
	end
	def getVirtualY
		100
	end
	def huhu
		puts "HUHU"
	end
	
end

class AntNewHouse<AntEntity
	def initialize
		super(Pos2D.new(0,0))
		@type=2
		setType("house")
		@men=[]
		#@res=Resource.new
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
	def getJob(e)
		pd=e.getPos2D-getPos2D
		n=pd.norm2
		puts "HOME:"+n.to_s+"   "+pd.to_s
		if n<30 then
			# is home:
			# 1) take everything from inventory
			resource.takeAll(e.resource)
			wood=resource.get("wood")
			puts "WOOOOOOOOOOOOOOOOOOOOOOOOOOOOD:"+wood.to_s
			# 2) give job
			if wood<5 then
				tree=getMap.getNext(self,"tree")
				if tree then
					e.setFetchTarget(tree,"wood")
					return MoveJob.new(0,tree.getPos2D,0,false)
				end
			else
				return RestJob.new(10)
			end
		else
			# is anywhere - come home
			return MoveJob.new(0,getPos2D,0,false)
		end
	end
end
