#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_house.rb
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

require 'ant_boss.rb'
require 'ant_tools.rb'

class AntFlag<AntMyEntity
	def initialize
		super(AGVector3.new(0,0,0))
		setType("flag")
		@age=0
	end
	def getTexture
		return "flag"
	end
end


class AntHouse<AntBoss
	def initialize
		super
		@type=3
		setType("house")
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		
		#mesh=Mesh.new(getMeshData("data/models/tower.ant2",3,"data/textures/models/tower_tex.png"),AGVector4.new(0,0,0),-30)
		#setMesh(mesh)
	end
	
	################################
	# Viewing,etc.
	################################
	
	# for recruiting
	def takeMan
		m=@men[0]
		removeMan(m)
		return m
	end
	
	######################################
	# EVENTS
	######################################
	
	def eventJobFinished
		#checkBirth
		newRestJob(2)
		eatFood
		if @fighting then
			checkFight
		end
	end
	
	def eventGotHLFight(hero)
		puts "IGNORING eventGotHLFight in ant_house"
	end
	
	def eatFood
		food=resource.get("food")
		eatAmount=@men.length*0.04
		if food>eatAmount
			resource.set("food",food-eatAmount)
		else
			resource.set("food",0)
			starve
		end
	end
	
	def starve
		puts "FIXME: STARVING"
	end
	
	def eventNoJob
		super
		eventJobFinished
	end
	
	def noHLJob
	end
	
	def setOwner(owner)
		super(owner)
		addFlag(owner)
	end
	
		
	def assignJob(e)
		if @fighting then
			checkFight
		elsif @job==nil then
			normalFetching(e)
		else
			checkHLJobEnd(e)
		end
	end
	
	# does assign job, too
	def checkHLJobEnd(man)
		if @job
			if @job.check(man) then 
				if @player
					@player.eventJobFinished(self,@job)
				end
				@job=nil 
			end
		end
	end
	
	def normalFetching(e)
		if atHome(e) then
			e.setMode("")
			e.setMeshState("stand")
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
		elsif e.getMode=~/fetching/
			res=e.getMode.gsub(/.* /,"")
			e.digResource(res)
			e.setMode("digging "+res)
		elsif e.getMode=~/digging/
			e.newMoveJob(0,getPos2D,0)#,false)
			res=e.getMode.gsub(/.* /,"")
			e.collectResource(res)
			e.setMode("homing")
		elsif e.getMode=="homing"
			e.newRestJob(1) # always rest a little
			e.setMode("")
		else
			# is anywhere - come home
			e.newMoveJob(0,getPos2D,0)#,false)
		end
	end
	
	# says if entity is at home
	def atHome(entity)
		pd=entity.getPos2D-getPos2D
		n=pd.length2 #norm2
		return n<30
	end
	
	# what's needed most ATM?
	# returns: [good,from] or nil
	def needed()
		goods={"wood"=>"tree","stone"=>"stone"}
		min=50
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
		# FIXME: check if tent has enough of good !!
	
	
		tent=getMap.getNext(self,enttype)
		if tent == nil then
			#puts "No '"+good+"' found!"
			ent.newRestJob(2)
		else
			ent.newFetchJob(0,tent,good)
			#ent.newMoveJob(0,tent.getPos2D)
			ent.setMode("fetching "+good)
		end
	end
	
	########################################
	# LOADING & SAVING
	########################################
	
	def loadXML(node)
		super(node)
		setPos(getPos2D) # set to ground
		if getName==""
			setName(rand.to_s)
		end
	end	
end
