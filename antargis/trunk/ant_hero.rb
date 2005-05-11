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

# try to implement alle entites in ruby
# WARNING: DON'T MEMBER_VARIABLES AS IT SEEMS TO CRASH RUBY SOMEHOW
# could be that it has something to do with Init_Stack ???

#GC.disable

require 'ents.rb'

class AntHLJob
end

class AntHeroMoveJob<AntHLJob
	def initialize(hero,prio,pos,dist)
		@hero=hero
		@prio=prio
		@pos=Pos2D.new(pos.x,pos.y)#clone
		@dist=dist
		
		format
	end
	def getMen()
		@hero.getMen
	end
	def format
		puts "FORMAT"
		men=getMen
		@dir=(@pos-@hero.getPos2D).normalized
		men.each{|x|
			fpos=@hero.getWalkFormation(x,@dir)+@hero.getPos2D
			x.newMoveJob(0,fpos,5)
		}
		check # if men.length==0
	end
	
	def checkReady
		#GC.disable # disable GC, as gets called too often here
		ready=true
		men=getMen
		men.each{|x|
			if x.hasJob then 
				ready=false
			else
				# check if is already there - don't know why this fails ATM
				is=x.getPos2D
				should=@hero.getWalkFormation(x,@dir)+@hero.getPos2D
				if (is-should).norm>10 then
					ready=false
					x.newMoveJob(0,should,@dist)
				end
			end
		}
		#GC.enable
		return ready
	end

	def check
		men=getMen
		ready=checkReady
		if ready then
			#getMap.pause
			# let all go
			@hero.newMoveJob(0,@pos,0)
			men.each{|x|
				fpos=@hero.getWalkFormation(x,@dir)+@pos
				x.newMoveJob(0,fpos,0)
			}
			return true
		end
		return false
	end
end

class AntHeroFightJob<AntHeroMoveJob
	def initialize(hero,target)
		@hero=hero
		@target=target
		@moveReady=false
		
		#@hero.newFightJob(0,target)
		super(hero,0,target.getPos2D,50) # near til 50 pixels
	end
	def check
		if not @moveReady then
			if super then
				@moveReady=true
			end
		end
		if @moveReady
			# fight
			puts "FIGHT!!!!!!!!!!!!!!!!!!!!!!!!!!"
			return true
		end
		return false
	end
end

class AntHeroRecruitJob<AntHeroMoveJob
	def initialize(hero,target,agg)
		@moveReady=false
		puts target
		puts target
		@target=target
		puts self
		puts "TARGET:"
		puts target
		puts @target
		puts "HUHU"
		@aggression=agg
		@targetMen=target.menCount
		puts @targetMen
		puts agg
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
			puts "TARGET:"
			puts @target
			man=@target.takeMan
			man.setBoss(@hero)
			@want=@want-1
		end
		return false
	end
	
end

class AntNewHero<AntMyEntity
	def initialize
		super(Pos2D.new(0,0))
		@men=[]
		#@resources={}
		setType("hero")
		@job=nil
		
		@createMen=0
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
		if node.get("men")!="" then
			@createMen=node.get("men").to_i
		end
	end
	
	def menCount
		return @men.length
	end
	
	def noJob
		if @createMen>0
			puts "CREATEMEN"
			for i in 0..(@createMen-1) do
				man=AntNewMan.new
				man.setPos2D(getPos2D)
				man.setVar("bossName",getVar("name"))
				getMap.insertEntity(man)
			end
			getMap.endChange
			@createMen=0
			puts "CREATEMEN READY"
		end
	end
	
	def getName
		return getVar("name")
	end
	
	def assignJob(e)
		if @job == nil then
			# rest job
			e=getMap.getRuby(e)
			formationPos=getSitFormation(e)
			if e.getPos2D==formationPos then
				e.newRestJob(5)
			else
				e.newMoveJob(0,formationPos,0)
			end
		else
			if @job.check then @job=nil end
		end
	end	
	
	def gotNewJob()
	end
	
	def newHLMoveJob(prio,pos,dist)
		puts "NEWMOVEJOB:"+pos.to_s
		@job=AntHeroMoveJob.new(self,prio,pos,dist)
	end
	def newHLRecruitJob(target)
		puts "NEWRECRUITJOB:"#+target.getName#pos.to_s
		puts "TARGET:"
		puts target
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
	
	def getMen
		@men
	end
	
	def signUp(man)
		puts "HEROSIGNUP"
		puts man
		@men.push(man)
	end
	# formation:
	# 1) wait for 3/4 of people are in formation but max. 5 seconds or so
	# 2) start all at once
	
	def getSitFormation(man)
		id=@men.index(man)
		
		if id then
			angle=id.to_f/@men.length*Math::PI*2
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
		id=@men.index(man)
		if id
			line=id/5
			col=id%5
			col=col-2
			normal=dir.normal
			l=dir*line*30
			c=normal*col*15
			
			return l+c
		else
			puts "ERROR in WalkFormation!"
			exit
		end
	end
	
end

