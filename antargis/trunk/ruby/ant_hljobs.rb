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
		@hero=getMap.getRuby(hero)
		@prio=prio
		@pos=AGVector2.new(pos.x,pos.y)
		@dist=dist
		@state="format"
		@states={"format"=>getMen,"move"=>[],"torest"=>[]}
		@formatDir=(@pos-@hero.getPos2D).normalized
		@hero.delJob
	end
	def getMen()
		@hero.getMen
	end

	def check(man)
		if not man
			return
		end
		case @state
			when "format"
				if man.getMode!="formating"
					f=@hero.getWalkFormation(man,@formatDir)+@hero.getPos2D
					man.newMoveJob(0,f,@dist)
					man.setMode("formating")
				else
					man.setMode("formatted")
					@states["move"].push(man)
					@states["format"].delete(man)
					if @states["format"].length==0
						# ok, all run at once
						getMen.each{|m|
							f=@pos+@hero.getWalkFormation(m,@formatDir)
							m.delJob
							m.newMoveJob(0,f,@dist)
							m.setMode("moving")
						}
						@state="move"
						@hero.newMoveJob(0,@pos,@dist)
					else
						man.newRestJob(1)
					end
				end
			when "move"
				man.newRestJob(1)
				@states["move"].delete(man)
				@states["torest"].push(man)
				if @states["move"].length==0
					# all to resting position
					getMen.each{|m|
						f=@pos+@hero.getSitFormation(m)-@hero.getPos2D
						m.newMoveJob(0,f,@dist)
						m.setMode("torest")
					}
					@state="torest"
				end
			when "torest"
				man.newRestJob(1)
				@states["torest"].delete(man)
		end
	end
	
	def finished
		@state=="torest" && @states["torest"].length==0
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
			d=(t.getPos2D-man.getPos2D).length
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
			man.setMode("rest")
			man.newRestJob(20)
		end
	end
	def finished
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
