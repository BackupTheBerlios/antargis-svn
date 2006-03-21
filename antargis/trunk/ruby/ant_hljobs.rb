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
		@men=getMen.clone
		@men.each{|m|
			m.setMode "format"
			m.delJob
		}
		
		@formatDir=(@pos-@hero.getPos2D).normalized
		@moveFinished=false
		@movingMen=0
	end
	def getMen()
		@hero.getMen
	end

	def startWalking
		@men.each{|m|
			f=@pos+@hero.getWalkFormation(m,@formatDir)
			#m.delJob
			m.newMoveJob(0,f,@dist)
			m.setMode("moving")
			@movingMen+=1
		}
		@state="move"
	end
	def getMenState(mode)
		#puts "getmenState"
		#@men.each{|m|puts m.getMode}
		@men.select{|m|m.getMode==mode}
	end

	def check(man)
		puts "CHECK (move-job): #{man.class.to_s} #{man.getName} #{man.getMode} state:#{@state}"
		case @state
			when "format"
				if man.getMode=="format"
					f=@hero.getWalkFormation(man,@formatDir)+@hero.getPos2D
					man.newMoveJob(0,f,@formatDist)
					man.setMode("formating")
				else
					man.setMode("formatted")
					man.newRestJob(1)
				end
				if getMenState("formating").length==0
					startWalking
				end
			when "move"
				case man.getMode
					when "moving"
						@movingMen-=1
						man.setMode("torest")
						if @movingMen>0
							man.newRestJob(5)
						end
				end
				puts "movingMen:#{@movingMen}"
				if @movingMen==0
					# FIXME - check if all men are really at destination
					puts "move finished!"
					@moveFinished=true
					#@state="torest"
				end
		end
	end
	
	def moveFinished
		return @moveFinished
	end
	
	def finished
		moveFinished
	end
		
end

class AntHeroFightJob<AntHeroMoveJob
	attr_reader :target
	#attr_writer :finished
	def initialize(hero,target,defend=false)

		puts "NEW ANTHEROFIGHT JOB #{hero.getName} #{target.getName}"

		@hero=hero
		@target=target
		@killStarted=false
		@killJobsGiven=false
		@defend=defend
		@hero.newRestJob(1)  #FIXME: this is an indirect method of killing actual job
		super(hero,0,target.getPos2D,10,(not defend)) # near til 10

		if @defend
			@state="fight"
		end
		@finished=false
		dputs "NEW:"
		dputs self
		dputs "defned:"+defend.to_s
		dputs @state
		@hero.assignJob2All
		initSitpositions
	end

	def finished
		@finished
	end
	
	def getEnergy
		raise "deprecated"
		e=0
		@men.each{|m|e+=m.getMorale}
		return e*0.1
	end
	
	def delete(man)
		@men.delete(man)
		#@states.each_key{|k|
		#	@states[k].delete(man)
		#}
	end

	def checkFlee
		dist=(@target.getPos2D-@hero.getPos2D).length
		if dist>10 and @state=="fight"
			puts "CHECKSTATE DIST:#{dist}"
			@finished=true
		end
	end
	
	def checkState
		checkFlee
		if moveFinished #or @defend
			puts "FightJob::checkState:MOVE FINISHED"
			@state="fight"
			@target.eventAttacked(@hero)
		end
		if @defend and @target.getJob.class!=self.class
			@finished=true # quick flee
		end
	end
	
	def check(man)
		puts "CHECKING #{man} #{man.getName} #{self} #{@hero.getName} #{@state}"
		checkState
		return if @finished
		case @state
			when "move","format"
				super(man)
				return false
			when "fight"
				if not @killStarted
					puts "killStarted #{@killStarted} #{self}"
					@target.eventGotHLFight(@hero)
					startFighting
					@killStarted=true
				end
				if man.getMode=="fight"
					return checkFight(man)
				else	
					man.newRestJob(2)
				end
		end
	end
	def defeated(man)
		playSound("ugh_end")
		puts "DEFEATED:"+man.to_s
		if man.is_a?(AntBoss)
			lost
			return
		end
		man.setMode("defeated")
		man.newMoveJob(0,@sitpos[man],0)
		#man.newMoveJob(0,AGVector2.new(0,0),0)
	end
	
	def lost
		if not @finished
			dputs "LOST!!!!!!!!!!"
			@finished=true
			@target.getJob.won
			@hero.setOwner(@target)
			@hero.killAllJobs
			@target.killAllJobs
			@hero.newHLMoveJob(0,@sitpos[@hero],0)
		end
	end
	def won
		if not @finished
			dputs "WON!!!!!!!!!!"
			playSound("won")
			@finished=true
			@target.getJob.lost
			@hero.killAllJobs
			@target.killAllJobs
			@hero.newHLMoveJob(0,@sitpos[@hero],0)
		end
	end

	
	def startFighting
		puts "START FIGHTING #{self} #{@hero.getName}"
		initSitpositions

		@hero.getMen.each{|man|
			man.setMode("fight")
			startFightingMan(man)
		}
	end
	
	def undefeatedMen
		return @men-getMenState("defeated")
	end
	
	def startFightingMan(man)
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
			puts "#{man.getName} attacks #{mtarget.getName}"
		else
			man.newRestJob(30)
		end
	end

	def haveDefeated(e)
		puts "haveDefeated: #{e.getName}"
		@men.each{|m|
			puts "checking: #{m.fightTarget} #{e}"
			if m.fightTarget==e
				puts "#{m.getName} not longer attacks #{e.getName}"
				m.delJob # search other target
			end
		}
	end
	
	def checkFightMan(man)
		if man
			if not man.hasJob
				startFightingMan(man)
			end
		end
	end
	
	def checkFight(man)
		if not @target.getJob.is_a?(AntHeroFightJob)
			return
		end
		tmen=@target.getJob.undefeatedMen
		# not won yet, so go on
		checkFightMan(man)
		
		return false
	end
private
	def initSitpositions
		# gather sitting positions
		@sitpos={}
		@men.collect{|man|
			@sitpos[man]=@hero.getSitFormation(man)
		}
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
		if moveFinished
			if man.class==AntHero
				if (not @state=~/recruit/)
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
						@wantedMen=@want+getMen.length-1

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
						puts "RESTTEST: #{@restingMen} #{@wantedMen}"
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


class AntHeroTakeFoodJob<AntHeroMoveJob
	attr_reader :finished
	def initialize(hero,target,agg)
		super(hero,0,target.getPos2D,4)
		@target=target
		@aggression=agg
		@want=[target.resource.get("food"),@men.length].min
		@oldpos=nil
	end

	def check(man)
		if moveFinished
			if man.class==AntHero	and @oldpos.class==NilClass
				@oldpos=man.getPos2D
			end
			case man.getMode
				when "takingFood"
					# take food and return
					@target.resource.sub("food",1)
					man.resource.add("food",1)

					@want-=1
					if @want<=0
						@finished=true
						@hero.newHLMoveJob(0,@oldpos,0)
					end
				else
					man.newMoveJob(0,@target.getPos2D,0)
					man.setMode("takingFood")
			end
		else
			super(man)
		end
	end
	
	
end


class AntHeroRestJob<AntHLJob
	def initialize(hero,time)
		@hero=hero
		@time=time
		@hero.newRestJob(time)
	end
	def check(man)
		formationPos=@hero.getSitFormation(man)
		diff=(man.getPos2D-formationPos)
		dist=diff.length2
		if dist<0.4 then
			if not ["sitdown","sit"].member?(man.meshState)
				man.sitDown
			else
				man.newRestJob(5)
				man.setMeshState("sit")
			end
		else
			puts "#{man.getPos2D} #{formationPos} #{dist} #{diff}"
			man.newMoveJob(0,formationPos,0)
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
