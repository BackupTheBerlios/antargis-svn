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

class AntFlag<AntRubyEntity
	def initialize
		super(AGVector3.new(0,0,0))
		setProvide("flag",true)
		@age=0
	end
	def getTexture
		return "flag"
	end
end


# AntHouse is the base class for all building types
# it provides functionality for:
# * defending
# * fetching resources
class AntHouse<AntBoss
	def initialize
		super
		@type=3
		setProvide("house",true)
		@defeated=[]
		@atHome=Set.new
		setMinimapColor(AGColor.new(0x55,0x55,0x55))
		setMinimapSize(5)
		
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

	# "normalFetching" is the current implementation for fetching resources of any kind
	# this includes:
	# * go to tree
	# * chop tree
	# * carry goods home
	# * rest some time
	# e: an entity - should be a man of this house
	def normalFetching(e)
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
				puts "normalFetch:#{self}"
				# keep at least a third of all men at home
				if need != nil and @atHome.length>@men.length/3 then
					fetch( need,e)
				else
					puts "nothin needed"
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
			# digging ready - take home
			e.newMoveJob(0,getPos2D,0)
			res=e.getMode.gsub(/.* /,"")
			e.collectResource(res)
			e.setMode("homing")
			# take resource
			amount=[e.target.resource.get(res),e.canCarry].min
			e.target.resource.sub(res,amount)
			e.resource.add(res,amount)
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
		tent=getMap.getNext(self,good,1)
		if tent == nil then
			puts "nothing found"
			ent.newRestJob(4)
		else
			puts "#{tent.getPlayer} #{getPlayer}"
			if tent.getPlayer!=getPlayer and (not tent.getPlayer.nil?)
			else
				ent.newMoveJob(0,tent.getPos2D,0.5)
				ent.setMode("fetching "+good)
				@atHome.delete(ent)
				ent.target=tent
				ent.setVisible(true)
			end
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


	# get a description for displaying in the info-box
	def getDescription
		m="man"
		home=_("{1} of them are at home.",@atHome.uniq.length)
		if getMen.length>1
			m="men"
		end

		r=_("This is a {1}. It was build {2} years ago. Currently {3} {4} live here.",_(houseType),age,getMen.length,_(m))+home

		morale=0
		@men.each{|m|morale+=m.getMorale}
		if @men.length>0
			morale/=@men.length
		end
		if morale>0.8
			s=_("in good mood.")
		elsif morale>0.5
			s=_("content.")
		elsif morale>0.2
			s=_("discontent.")
		else
			s=_("bad tempered.")
		end
		r+=_("They are {1}",_(s))
		r
	end

protected
	def houseType
		"house"
	end

private
	# check if my man has enough food otherwise I'll feed him
	def checkFood(man)
		if man.getFood<0.5
			# eat
			if resource.get("food")>1
				man.incFood(1)
				resource.sub("food",1)
			end
		end
	end

	# add a new flag when owner has changed - currently not implemented
	def addFlag(owner)
	end

end
