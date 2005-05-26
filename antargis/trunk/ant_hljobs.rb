#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hljobs.rb
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

require 'ents.rb'

class AntHLJob
end

class AntHeroMoveJob<AntHLJob
	def initialize(hero,prio,pos,dist,doFormat=true)
		@hero=hero
		@prio=prio
		@pos=Pos2D.new(pos.x,pos.y)#clone
		@dist=dist
		@myMoveReady=false
		@sentFormat=[]
		@readyFormat=[]
		if doFormat
			startFormation
		end
	end
	def getMen()
		@hero.getMen
	end
	
	def checkFormationMan(man)
		if (not man.hasJob) then 
			# check if is already there - don't know why this fails ATM
			is=man.getPos2D
			should=@hero.getWalkFormation(man,@formatDir)+@hero.getPos2D
			if (is-should).norm>10 then
				man.newMoveJob(0,should,0)
			else
				man.newRestJob(2) # wait for 2 seconds
				return true
			end
		end
		return false
	end
	
	def sendFormationMan(man)
		fpos=@hero.getWalkFormation(man,@formatDir)+@hero.getPos2D
		if not @sentFormat.member?(man)
			@sentFormat.push(man)
		end
	end
	
	def startFormation
		# let hero rest for some time
		@hero.newRestJob(2)
		men=getMen
		@formatDir=(@pos-@hero.getPos2D).normalized
		men.each{|man|
			sendFormationMan(man)
		}
		
		#check(nil) # if men.length==0, check wouldn't be called otherwise????
	end
	
	def checkReady(man)
		if man
			if not @readyFormat.member?(man)
				if checkFormationMan(man)
					@sentFormat.delete(man)
					@readyFormat.push(man)
				end
			end
		end
		if @sentFormat.length+@readyFormat.length!=@hero.menCount
			puts "RESTART FORMATION!!!!!!!!!!!!!!!"
			startFormation # redo from start
		end
		return @sentFormat.length==0
	end
	
	def checkReadyOld
		#GC.disable # disable GC, as gets called too often here
		ready=true
		men=getMen
		men.each{|x|
			if x.hasJob then 
				ready=false
			else
				# check if is already there - don't know why this fails ATM
				is=x.getPos2D
				should=@hero.getWalkFormation(x,@formatDir)+@hero.getPos2D
				if (is-should).norm>10 then
					ready=false
					x.newMoveJob(0,should,@dist)
				end
			end
		}
		#GC.enable
		return ready
	end
	
	def check(man)
		if @myMoveReady==false
			if checkMove(man)
				@myMoveReady=true
			end
		else
			return moveEnded
		end
		return false
	end

	def checkMove(man)
		men=getMen
		ready=checkReady(man)
		if ready then
			#getMap.pause
			# let all go
			@hero.newMoveJob(0,@pos,@dist)
			men.each{|x|
				fpos=@hero.getWalkFormation(x,@formatDir)+@pos
				x.newMoveJob(0,fpos,@dist)
			}
			return true
		end
		return false
	end
	
	def moveEnded
		men=getMen
		men.each{|x|
			if x.hasJob then
				return false
			end
		}
		return true
	end
end

class AntHeroFightJob<AntHeroMoveJob
	def initialize(hero,target,defend=false)
		@hero=hero
		@target=target
		if defend
			@moveReady=true
			puts "DEFENDING:::::::::::::::::::::::::::::::::::"
		else
			puts "ATTACKING!!!!!!!!!!!!!"
			@moveReady=false
		end
		@killStarted=false
		@killJobsGiven=false
		@defend=defend
		#@hero.newFightJob(0,target)
		@hero.newRestJob(1)  #FIXME: this is an indirect method of killing actual job
		super(hero,0,target.getPos2D,400,(not defend)) # near til 400 pixels
	end
	def check(man)
		if not @moveReady then
			if super(man) then
				@moveReady=true
			end
		end
		if @moveReady
			if not @killstart
				@target.eventGotHLFight(@hero)
				startFighting
			end
			@killStarted=true
			return checkFight(man)
		end
		return false
	end
	
	def startFighting
		@hero.getMen.each{|man|
			startFightingMan(man)
		}
	end
	
	def startFightingMan(man)
		tmen=@target.undefeatedMen
		# search nearest enemy
		dist=10000.0
		mtarget=nil
		tmen.each{|t|
			d=(t.getPos2D-man.getPos2D).norm
			if d<dist
				dist=d
				mtarget=t
			end
		}
		if mtarget then
			man.newFightJob(0,mtarget)
		end
	end
	
	def target
		@target
	end
	
	def checkFightMan(man)
		if man
			if not man.hasJob
				startFightingMan(man)
			end
		end
	end
	
	def checkFight(man)
		#puts "CHECKFIGHT"
		tmen=@target.undefeatedMen
		umen=@hero.undefeatedMen
		#puts umen.length
		#puts tmen.length
		if umen.length==0 then
			@hero.lostFight(@target)
			puts "LOST!!!!!!!!!!"
			return true # end this job
		elsif tmen.length==0
			puts "WON!!!!!!!!!!!"
			@hero.wonFight(@target)
			return true # end this job
		else # not won yet, so go on
			checkFightMan(man)
			if not @hero.hasJob
				@hero.newRestJob(3)
			end
			
			if @killJobsGiven==false and @defend==false then
				# set target fighting,too
				@target.newHLDefendJob(@hero)
			end
			@killJobsGiven=true
		end
		return false
	end

end

class AntHeroRecruitJob<AntHeroMoveJob
	def initialize(hero,target,agg)
		@moveReady=false
		@target=target
		@aggression=agg
		@targetMen=target.menCount
		@want=@targetMen*agg/3
		super(hero,0,target.getPos2D,40)
	end
	
	def check
		if not @moveReady then
			if super then
				@moveReady=true
			end
		end
		if @moveReady
			# recruit
			if @want==0 then 
				return true 
			end
			man=@target.takeMan
			man.setBoss(@hero)
			@want=@want-1
		end
		return false
	end
	
end

class AntHeroRestJob
	def initialize(hero,time)
		@hero=hero
		@time=time
		@hero.newRestJob(time)
	end
	def check(man)
		if man
			man.newRestJob(20)
		end
		return (not @hero.hasJob)
	end
end


class AntHouseFetchJob
	def initialize(house)
		@house=house
	end
	def check
	end
end
