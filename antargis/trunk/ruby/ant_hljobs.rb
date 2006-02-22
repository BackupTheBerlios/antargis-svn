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
	def undefeatedMen
		@hero.getMen
	end
	def delete(man)
	end
end

class AntHeroMoveJob<AntHLJob
	def initialize(hero,prio,pos,dist,doFormat=true)
		@hero=hero #getMap.getRuby(hero)
		@hero.delJob
		@prio=prio
		@pos=AGVector2.new(pos.x,pos.y)
		@dist=dist
		@formatDist=0
		if getMen.length>0
			@state="format"
		else
			@state="torest"
			@hero.newMoveJob(0,@pos,@dist)
		end
		@states={"format"=>getMen,"move"=>[],"torest"=>[]}
		@formatDir=(@pos-@hero.getPos2D).normalized
	end
	def getMen()
		@hero.getMen
	end

	def check(man)
		if man.class!=AntNewMan
			return
		end
		case @state
			when "format"
				if man.getMode!="formating"
					f=@hero.getWalkFormation(man,@formatDir)+@hero.getPos2D
					man.newMoveJob(0,f,@formatDist)
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
				#man.sitDown
				@states["torest"].delete(man)
		end
	end
	
	def moveFinished
		@state=="torest" && @states["torest"].length==0
	end
	
	def finished
		moveFinished
	end
		
end

class AntHeroFightJob<AntHeroMoveJob
	attr_reader :finished, :target
	#attr_writer :finished
	def initialize(hero,target,defend=false)
		@hero=hero
		@target=target
		@killStarted=false
		@killJobsGiven=false
		@defend=defend
		@hero.newRestJob(1)  #FIXME: this is an indirect method of killing actual job
		super(hero,0,target.getPos2D,10,(not defend)) # near til 10
		@men=hero.getMen.clone
		@states["fight"]=[]
		@states["defeated"]=[]
		if @defend
			@state="fight"
			@states["fight"]=@men.clone
		end
		@finished=false
		dputs "NEW:"
		dputs self
		dputs "defned:"+defend.to_s
		dputs @state
		@hero.assignJob2All
	end
	
	def getEnergy
		if @men.length==0
			raise "ERROR"
		end
		return @states["fight"].length.to_f/@men.length
	end
	
	def delete(man)
		@states.each_key{|k|
			@states[k].delete(man)
		}
	end
	
	def checkState
		if @state=="torest" #moveFinished
			dputs "changed state:"+self.to_s
			@states["fight"]+=@states["torest"]
			@states["fight"].uniq!
			@state="fight"
			@target.eventAttacked(@hero)
		end
	end
	
	def check(man)
		checkState
		dputs "CHECKING:"+man.to_s+"   "+self.to_s+"  "+@state+"   myHero:"+@hero.to_s
		case @state
			when "move","format"
				if super(man)
					@states["fight"].push(man)
				end
				return false
			when "fight"
				if not @killStarted
					@target.eventGotHLFight(@hero)
					startFighting
					@killStarted=true
				end
				if @states["fight"].member?(man)
					return checkFight(man)
				elsif man
					@hero.moveHome(man)
				end
		end
	end
	def defeated(man)
		playSound("ugh_end")
		dputs "sigdefeat"
		dputs self
		@states["fight"].delete(man)
		dputs "FIGHTING:",@states["fight"]
		dputs "FIGHTING:",@states["fight"].length
		if @states["fight"].length==0
			lost
		end
	end
	
	def lost
		if not @finished
			dputs "LOST!!!!!!!!!!"
			@finished=true
			#@hero.lostFight(@target)
			#@target.wonFight(@hero)
			@target.getJob.won
			@hero.setOwner(@target)
			@hero.killAllJobs
			@target.killAllJobs
		end
	end
	def won
		if not @finished
			dputs "WON!!!!!!!!!!"
			playSound("won")
			@finished=true
			@target.getJob.lost
			#@hero.wonFight(@target)
			#@target.lostFight(@hero)
			@hero.killAllJobs
			@target.killAllJobs
		end
	end
	
	def startFighting
		@hero.getMen.each{|man|
			startFightingMan(man)
		}
	end
	
	def undefeatedMen
		@men-@states["defeated"]
	end
	
	def startFightingMan(man)
		if @target.getJob.class!=self.class
			@target.newHLDefendJob(@hero)
		end
		tmen=@target.getJob.undefeatedMen
		# search nearest enemy
		dist=1000.0
		mtarget=nil
		tmen.each{|t|
			if t.getEnergy>0 and t.dead!=true
				d=(t.getPos2D-man.getPos2D).length
				if d<dist
					dist=d
					mtarget=t
				end
			end
		}
		if mtarget then
			if mtarget.getEnergy<=0
				raise "ERROR in startFightingMan"
			end
			man.newFightJob(0,mtarget)
		else
			man.newRestJob(30)
		end
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
		tmen=@target.getJob.undefeatedMen
		umen=@hero.getJob.undefeatedMen
		#puts umen.length
		#puts tmen.length
		if umen.length==0 then
			lost
			return true # end this job
		elsif tmen.length==0
			won
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
	attr_reader :finished
	def initialize(hero,target,agg)
		@target=target
		@aggression=agg
		@targetMen=target.menCount
		@want=@targetMen*agg/3
		@finished=false
		@restingMen=0
		@wantedMen=@want
		super(hero,0,target.getPos2D,4)
	end

	def check(man)
		if @state=="wait_recruit" or @state=="torest"
			if man.class==AntHero
				if @state=="torest"
					if @want==0 then 
						@state="wait_recruit"
					else
						man.newRestJob(1)
						nman=@target.takeMan
						nman.setBoss(@hero)
						nman.setMode("to_recruit")
						@want=@want-1

						# FIXME:
						getMen.each{|m|
							m.setMode("to_recruit")
						}
						@restingMen=0
						@wantedMen=@want+getMen.length

					end
				else
					super(man)
				end
			else
				case man.getMode
					when "to_recruit"
						fp=@hero.getSitFormation(man)
						man.newMoveJob(0,fp,0)
						man.setMode("recruit_torest")
					when "recruit_torest"
						man.newRestJob(1)
						man.setMode("recruit_resting")
						@restingMen+=1
						if @restingMen==@wantedMen
							@finished=true
						end
					when "recruit_resting"
						man.newRestJob(1)
				end
			end
		else
			super(man)
		end
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
