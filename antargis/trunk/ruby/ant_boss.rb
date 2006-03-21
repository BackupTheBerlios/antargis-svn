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
		setProvide("boss",true)
		setupMeshBoss
		@selected=@hovered=false
	end
	def loadXML(node)
		super(node)
		if node.get("men")!="" then
			dputs "LOAD:CREATING MEN:"+node.get("men")
			if node.get("men")!=""
				@createMen=node.get("men").to_i
			end
		end
	end
# for recruiting
	def takeMan
		m=@men[-1]
		removeMan(m)
		return m
	end
		
	def getJob
		@job
	end
	
	def menCount
		@men=@men.uniq
		return @men.length
	end
	
	def eventNoJob
		puts "eventNoJob "+self.class.to_s+" "+@job.to_s
		checkHLJobEnd(self)
 		checkCreateMen
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
		puts "DEFEND!!! #{self.getName} from #{target.getName}"
		if @job
			if @job.is_a?(AntHeroFightJob)
				puts "I'm alread fighting!"
				return # already fighting
			end
		end
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
		if @job
			@job.delete(man)
		end
	end
	
	def setPlayer(player)
		if @player
			@player.remove(self)
		end
		@player=player
	end
	def getPlayer
		@player
	end
	
	def eventGotHLFight(hero)
		newHLDefendJob(hero)
		return
	end
	
	def eventManDefeated(man)
		dputs "AntBoss:eventManDefeated: #{man} #{man.getName}"
		if @job and @job.class==AntHeroFightJob
			@job.defeated(man)
		end
	end

	def getBossEnergy
		if @job and @job.class==AntHeroFightJob
			return @job.getEnergy
		else
			e=0
			@men.each{|m|e+=m.getMorale}
			return e*0.1
		end
	end


	def setOwner(owner)
		@owner=owner
		dputs "RESETING PLAYER:"
		if @player
			@player.remove(self)
		end
		@player=owner.getPlayer
		@player.add(self)
		getMap.eventOwnerChanged(self)
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
	
	
	# ring mesh
	def setupMesh
	end
	
	def hovered=(s)
		@hovered=s
		#puts @hovered,@selected
		@ring.setVisible((@hovered or @selected))
		if @hovered and not @selected
			@ring.setColor(AGVector4.new(0.7,0.7,1,0.8))
		end
	end
	def selected=(s)
		@selected=s
		puts @hovered,@selected
		@ring.setVisible((@hovered or @selected))
		if @selected
			@ring.setColor(AGVector4.new(1,0.7,0.1,0.8))
			#@ring.setColor(AGVector4.new(1,0.7,1,0.8))
		end
	end

		
	def getRing
		makeBigRingMesh
	end
	def setupMeshBoss
		setupMesh
		@ring=getRing
		if @selected
			#f6c108
			@ring.setColor(AGVector4.new(1,0.7,0.1,0.8))
		else
			@ring.setColor(AGVector4.new(0.7,0.7,1,0.8))
		end
		addMesh(@ring,AGVector3.new(0,0,0))
		@ring.setVisible(false)
	end

	def eventHLJobFinished(job)
		getMap.eventHLJobFinished(self,job)
	end

	def checkHLJobEnd(man)
		if @job
			if man
				@job.check(man)
			end
			if @job.finished then 
				eventHLJobFinished(@job)
				if @player
					@player.eventJobFinished(self,@job)
				end
				@job=nil 
			end
		end
		if @job==nil
			noHLJob
		end
	end
	def setAggression(a)
		super
		getMen.select{|m|(not m.is_a?(AntBoss))}.each{|m|m.setAggression(a)}
	end	
	def getSitFormation(man)
		return getPos2D
	end

	def eventHaveDefeated(e)
		puts "#{getName} has defeated #{e.getName}"
		if @job.is_a?(AntHeroFightJob)
			@job.haveDefeated(e)
		end
	end
end


