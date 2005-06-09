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

class AntFlag<AntMyEntity
	def initialize
		super(Pos3D.new(0,0,0))
		setType("flag")
		@age=0
	end
	def getTexture
		return "flag"
	end
end


class AntNewHouse<AntNewBoss #MyEntity
	def initialize
		super #(Pos2D.new(0,0))
		@type=3
		setType("house")
		@men=[]
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		
		setVirtualY(250)
	end
	
	###############################
	# FLAG support
	###############################
	
	# override this for reseting the flag's position
	def setPos2D(p)
		super(p)
		if @flag then
			p=getPos3D
			p.y+=290
			p.z-=150
			@flag.setPos3D(p)
		end
	end
	
	def addFlag(owner)
		@flag=AntFlag.new
		setPos2D(getPos2D) # reset flag position
		$map.insertEntity(@flag)
	end
	
	
	################################
	# SIGN UP & REMOVE MAN
	################################
	
	#def signUp(man)
	#	puts "SIGNUP"
	#	puts man
	#	if not @men.include?(man)
	#		@men.push(man)
	#	end
	#end
	
	#d#ef removeMan(man)
	#	@men.delete(man)
	#	@atHome.delete(man)
	#end
	
	################################
	# Viewing,etc.
	################################
	
	def setHouseType(t)
		@type=t
	end
	def getTexture
		puts "HOUSE::getTexture"
		return "tower"+@type.to_s
	end
	
	def getMen
		@men
	end
	
	def menCount
		#@men=@men.uniq
		@men.length
	end
	
	def takeMan
		m=@men[0]
		removeMan(m)
		return m
	end
	
	def checkBirth
		# only men, which are at home can add to birth rate
		@lastBirth+=[@atHome.length,30].min
		puts "LASTBIRTH:"+@lastBirth.to_s
		if @lastBirth>50 then
			puts "A MAN IS BORN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
			man=AntNewMan.new
			getMap.insertEntity(man)
			man.setPos2D(getPos2D)
			getMap.endChange
			@lastBirth=-10*rand
		end
	end

	
	######################################
	# EVENTS
	######################################
	
	def eventJobFinished
		checkBirth
		newRestJob(2)
		if @fighting then
			checkFight
		end
	end
	
	def eventGotHLFight(hero)
		puts "IGNORING eventGotHLFight in ant_house"
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
	
	########################################
	# LOADING & SAVING
	########################################
	def xmlName
		return "antNewHouse"
	end
	
	def loadXML(node)
		super(node)
		setPos2D(getPos2D)
	end	
end
