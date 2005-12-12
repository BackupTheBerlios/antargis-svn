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


class AntBoss<AntMyEntity
	def initialize
		super(AGVector2.new(0,0))
		@men=[]
		@job=nil
		@createMen=0
	end
	def loadXML(node)
		super(node)
		if node.get("men")!="" then
			puts "LOAD:CREATING MEN:"+node.get("men")
			if node.get("men")!=""
				@createMen=node.get("men").to_i
			end
		end
	end
	
	def getJob
		@job
	end
	
	def menCount
		@men=@men.uniq
		return @men.length
	end
	
	def eventNoJob
		checkHLJobEnd(nil)
		checkCreateMen
		if @job && @job.finished
			@job=nil
		end
		if @job==nil
			noHLJob
		end
	end
	
	def checkCreateMen
		if @createMen>0
			for i in 0..(@createMen-1) do
				man=AntNewMan.new
				getMap.insertEntity(man)
				man.setPos(getPos2D)
				man.setBoss(self)
			end
			getMap.endChange
			@createMen=0
		end
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
		@men.clone
	end
	
	def signUp(man)
		if not @men.member?(man) then	
			@men.push(man)
		end
	end
	
	def removeMan(man)
		@men.delete(man)
		@jobs.delete(man)
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
		if @job and @job.class==AntHeroFightJob
			@job.defeated(man)
		end
	end
	def setOwner(owner)
		@owner=owner
		puts "RESETING PLAYER:"
		@player=owner.getPlayer
	end
	def getOwner
		@owner
	end
	
	def assignJob2All
		@men.each{|man|
			man.delJob
			assignJob(man)
		}
	end
	def killAllJobs
		@men.each{|man|man.delJob}
	end
end

