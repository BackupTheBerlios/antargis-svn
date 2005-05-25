#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_boss.rb
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


class AntNewBoss<AntMyEntity
	def initialize
		super(Pos2D.new(0,0))
		@men=[]
		@job=nil
		@defeated=[]
		@createMen=0
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
	
	def eventNoJob
		checkHLJobEnd(nil)
		
		puts "EVENTNOJOB"
		puts "NAME:"
		puts getName
		if @createMen>0
			for i in 0..(@createMen-1) do
				puts "HERO::CREATING MEN:"
				man=AntNewMan.new
				puts "READY CREATING"
				getMap.insertEntity(man)
				man.setPos2D(getPos2D)
				man.setBoss(self)
				puts "CREATING MEN"
			end
			getMap.endChange
			@createMen=0
		end
		if @job==nil
			#puts "noHLJOB"
			noHLJob
		end
		if @job
			puts "JOB:"
			puts @job.class
		end
		puts "EVNETNOJOB!"
	end
	
	
	def gotNewJob()
	end
	
	def newHLDefendJob(target)
		@job=AntHeroFightJob.new(self,target,true) # FIXME: change this, so that fighting is stopped as soon as other stops
	end
	def newHLRestJob(time)
		@job=AntHeroRestJob.new(self,time)
	end
	
	def getMen
		@men
	end
	
	def signUp(man)
		puts "SIGNUP"
		puts man
		if @men.member?(man) then	
			puts "ALREADY INSERTED!"
			puts "IGONRING"
		else
			@men.push(man)
		end
		
	end
	
	def removeMan(man)
		@men.delete(man)
		if @defeated.include?(man)
			@defeated.delete(man)
		end
	end
	
 	def undefeatedMen
		return @men-@defeated
	end

	def defeatedMen
		return @defeated
	end
	
	def setPlayer(player)
		@player=player
	end
	def getPlayer
		@player
	end
	
	def eventGotHLFight(hero)
		return
	end
	
	def eventManDefeated(man)
		puts "SIGDEFEATED"
		@defeated.push man
	end
	def wonFight(hero)
		@job=nil
		resetClientJobs
	end
	def lostFight(hero)
		setOwner(hero)
		@job=nil
		resetClientJobs
	end
	def resetClientJobs
		@defeated=[]
		@men.each{|man|
			man.newRestJob(1)
		}
	end
		
	def setOwner(owner)
		@owner=owner
		puts "RESETING PLAER:"
		@player=owner.getPlayer
	end
	def getOwner
		@owner
	end
end

