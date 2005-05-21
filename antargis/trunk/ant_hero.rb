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


class AntNewHero<AntMyEntity
	include FightHLEntity

	def initialize
		super(Pos2D.new(0,0))
		@men=[]
		#@resources={}
		setType("hero")
		@job=nil
		@defeated=[]
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
			for i in 0..(@createMen-1) do
				man=AntNewMan.new
				man.setPos2D(getPos2D)
				man.setVar("bossName",getVar("name"))
				getMap.insertEntity(man)
			end
			getMap.endChange
			@createMen=0
		end
		if @player
			@player.assignJob(self)
		end
	end
	
	def getName
		return getVar("name")
	end
	
	def assignJob(e)
		if @fighting then
			checkFight
		elsif @job == nil then
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
	
	def getMen
		@men
	end
	
	def signUp(man)
		@men.push(man)
	end
	
	def removeMan(man)
		@men.delete(man)
		if @defeated.include?(man)
			@defeated.delete(man)
		end
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
	
 	def undefeatedMen
		return @men-@defeated
	end

	
	def getWalkFormation(man,dir)
		#setTrap
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
	
	def setPlayer(player)
		@player=player
	end
end

