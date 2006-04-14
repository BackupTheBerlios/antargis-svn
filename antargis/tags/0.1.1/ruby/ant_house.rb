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
		setProvide("flag",true)
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
		setProvide("house",true)
		@defeated=[]
		@atHome=Set.new
		@lastBirth=0
		
	end
	
	################################
	# Viewing,etc.
	################################
	
	######################################
	# EVENTS
	######################################
	
	def eventJobFinished
		#checkBirth
		newRestJob(2)
		eatFood
		process
	end

	def process
	end
	
	def eventGotHLFight(hero)
		puts "IGNORING eventGotHLFight in ant_house"
	end
	
	def eventAttacked(by)
		newHLDefendJob(by)
	end
	
	def moveHome(man)
		pos=getPos2D
		if (man.getPos2D-pos).length>1
			man.newMoveJob(0,pos,0)
		else
			man.newRestJob(2)
		end

	end
	
	def eatFood
		return 
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
		if @job==nil then
			normalFetching(e)
		else
			checkHLJobEnd(e)
		end
	end
	
	# does assign job, too
	def checkHLJobEnd(man)
		if @job
			@job.check(man) 
			if @job.finished then 
				if @player
					@player.eventJobFinished(self,@job)
				end
				@job=nil 
			end
		end
	end

	
	def normalFetching(e)
		puts "FECTHIGN"
		if atHome(e) then
			checkFood(e)
			@atHome.push(e)
			if e.getMode=="resting"
				e.setMode("")
				e.setMeshState("stand")
				# is home:
				# 1) take everything from inventory
				resource.takeAll(e.resource)
				# 2) give job
				need=needed()
				# keep at least a third of all men at home
				if need != nil and @atHome.length>@men.length/3 then
					fetch( need,e)
				else
					e.newRestJob(5+getRand)
					e.setVisible(false)
				end
			else
				e.setMode("resting")
				e.newRestJob(5+getRand)
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
		return n<1
	end
	
	# what's needed most ATM?
	# returns: [good,from] or nil
	def needed()
		goods=["wood","stone","food"]
		minarg=goods.min {|a,b|resource.get(a)<=>resource.get(b)}
		minval=resource.get(minarg)
		if minval>=50
			return nil
		end
		return minarg
	end


	# assigns ent a job for fetching good from a enttype
	def fetch(good,ent)
		# FIXME: check if tent has enough of good !!
	
		puts self, good,good.class
		tent=getMap.getNext(self,good)
		if tent == nil then
			#puts "No '"+good+"' found!"
			ent.newRestJob(10)
		else
			ent.newFetchJob(0,tent,good)
			#ent.newMoveJob(0,tent.getPos2D)
			ent.setMode("fetching "+good)
			@atHome.delete(ent)
			ent.setVisible(true)
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
	def addFlag(owner)
	end
	def getDescription
		"This is a keep. It was build #{age} years ago. Currently #{getMen.length} men live here. #{@atHome.length} of them are at home."
	end

	private
	def checkFood(man)
		if man.getFood<0.5
			# eat
			if resource.get("food")>1
				man.incFood(1)
				resource.sub("food",1)
			end
		end
	end

end
