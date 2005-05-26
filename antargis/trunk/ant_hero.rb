#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hero.rb
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
require 'ant_hljobs.rb'
require 'ant_boss.rb'

class AntNewHero<AntNewBoss
	def initialize
		super
		setType("hero")
	end
	def getTexture
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
	
	def checkHLJobEnd(man)
		if @job
#			puts "CHECKHLJOBENDED:"
#			puts @job
			if @job.check(man) then 
				if @player
					@player.eventJobFinished(self,@job)
				end
				@job=nil 
			end
		end
	end
	
	def noHLJob
		puts self
		puts getName
		puts "noHLJob1"
		if @player
			puts "noHLJOB"
			@player.assignJob(self)
		else
			puts "no Player Foudn!"
			puts "wait 5 seconds"
			# rest
			newHLRestJob(5)
		end
	end
	
	#def getName
	#	return getVar("name")
	#end
	
	def assignJob(e)
		if @fighting then
			checkFight
		elsif @job == nil or @job.class==AntHeroRestJob then
			# rest job
			e=getMap.getRuby(e)
			formationPos=getSitFormation(e)
			if e.getPos2D==formationPos then
				e.newRestJob(5)
			else
				e.newMoveJob(0,formationPos,0)
			end
		else
			checkHLJobEnd(e)
		end
	end	
	
	def gotNewJob()
	end
	
	def newHLMoveJob(prio,pos,dist)
		@job=AntHeroMoveJob.new(self,prio,pos,dist)
	end
	def newHLRecruitJob(target)
		@job=AntHeroRecruitJob.new(self,target,$buttonPanel.getAggression)
	end
	def newHLFightJob(target)
		@job=AntHeroFightJob.new(self,target)
	end
	
	def newHLDismissJob()
		agg=$buttonPanel.getAggression
		c=menCount*agg/3
		men=@men[0..c]
		men.each{|m|
			m.setNoBoss
			@men.delete(m)
		}
	end
	
	
	# formation:
	# 1) wait for 3/4 of people are in formation but max. 5 seconds or so
	# 2) start all at once
	
	def getSitFormation(man)
		id=@men.index(man)
		
		if id then
			angle=id.to_f/@men.length*Math::PI*2
			#puts "SITTING:"
			#puts id
			#puts @men.length
			radius=40
			return Pos2D.new(Math::sin(angle)*radius,Math::cos(angle)*radius)+getPos2D
		else
			puts "ERROR in SitFormation!"
			puts "MEN:"+@men.to_s
			puts "man:"+man.to_s
			exit
		end
	end
	
	def getWalkFormation(man,dir)
		#setTrap
		id=@men.index(man)
		if id
		
			lineWidth=30
			if @men.length>30
				lineWidth=15
			end
		
			if id>=2 then
				id+=1 # exclude hero's position
			end
			line=id/5
			col=id%5
			col=col-2
			normal=dir.normal
			l=dir*line*lineWidth
			c=normal*col*15
			
			return l+c
		else
			puts "ERROR in WalkFormation!"
			exit
		end
	end
	
	def eventGotHLFight(hero)
		return
		job=true
		if @job
			job=false
			if @job.class ==AntHeroFightJob
				if @job.target==hero then
					job=true
				end
			end
		end
		if job
			newHLFightJob(hero)
		end
	end
end

