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

require 'entities.rb'
require 'ant_hljobs.rb'
require 'ant_formations.rb'


class AntBoss<AntRubyEntity
	attr_accessor :formation

	attr_accessor :hlJobMode

	def initialize
		super(AGVector2.new(0,0))
		@men=[]
		@hlJobMode={}
		@job=nil
		@createMen=0
		@selected=@hovered=false
		@formation=nil
		setProvide("boss",true)
		setupMeshBoss
		setAggression(3)
	end
	def loadXML(node)
		super(node)
		if node.get("men")!="" then
			dputs "LOAD:CREATING MEN:"+node.get("men")
			dputs caller.join("\n")
			if node.get("men")!=""
				@createMen=node.get("men").to_i
			end
		end
		node.getChildren("hljob").each{|n|
			t=n.get("type")
			case t
				when "AntHeroMoveJob"
					@job=AntHeroMoveJob.new(self,0,AGVector2.new(0,0),0)
				when "AntHeroRestJob"
					@job=AntHeroRestJob.new(self,10)
				when "AntHeroRecruitJob"
					target=n.getChildren.select{|c|c.get("name")=="@target"}[0]
					if target
						target=getMap.getByUID(target.get("value"))
					
						@job=AntHeroRecruitJob.new(self,target)
					else
						raise "target missing in saved AntHeroRecruitJob"
					end
				else
					# FIXME!!!
					raise "unknown job: #{t}"
			end
			@job.loadXML(n)
		}
		setAggression(getAggression) # update aggression of men
	end
# for recruiting
	def takeMan
		men=@men.select{|m|not m.is_a?(AntBoss)}
		if men.length==0
			return nil
		end
		m=men[-1]
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
# 		dputs "eventNoJob "+self.class.to_s+" "+@job.to_s
# 		dputs caller.join("\n")
		checkHLJobEnd(self)
 		checkCreateMen
	end

	def checkCreateMen
		if @createMen>0
			for i in 0..(@createMen-1) do
				puts "NEW ---------MAN"
				man=AntMan.new
				getMap.insertEntity(man)
				man.setPos(getPos2D)
				man.setBoss(self)
			end
			#getMap.endChange
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
				return false # already fighting
			end
		end
		@job=AntHeroFightJob.new(self,target,true) # FIXME: change this, so that fighting is stopped as soon as other stops
		assignJob2All
		return true
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
			man.setAggression(getAggression)
			if @job.is_a?(AntHeroRestJob)
				newHLRestJob(1)
			elsif @job
				@job.check(man)
			end
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
		if @player
			@player.add(self)
		end
	end
	def getPlayer
		@player
	end
	
# 	def eventGotHLFight(hero)
# 		newHLDefendJob(hero)
# 		return
# 	end

	def eventAttacked(by)
		ret=newHLDefendJob(by)
		AntInventory.update(self)
		ret
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

	def underAttack
		(@job.is_a?(AntHeroFightJob) and (not @job.finished))
	end


	def setOwner(owner)
		trace
		@owner=owner
		dputs "RESETING PLAYER:"
		if @player
			@player.remove(self)
		end
		@player=owner.getPlayer
		if @player
			@player.add(self)
		end
		getMap.eventOwnerChanged(self)
		trace
		AntInventory.update(self)
		trace
		#resourceChanged
	end
	def getOwner
		@owner
	end
	
	def assignJob2All
		if getMap
			getMap.eventNewJobAssignedToBoss(self) if getMap.respond_to?(:eventNewJobAssignedToBoss)
		end

		begin
		# discard reassigning in this case
		return if ObjectSpace.const_get(:AntNewHLJob)
		rescue
		end

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
		updateRingColor
		return
		#puts @hovered,@selected
		@ring.setVisible((@hovered or @selected))
		if @hovered and not @selected
			@ring.setRingColor(AGVector4.new(0.7,0.7,1,0.8))
		end
	end
	def selected=(s)
		@selected=s
		updateRingColor
		return
		puts @hovered,@selected
		@ring.setVisible((@hovered or @selected))
		if @selected
			@ring.setRingColor(AGVector4.new(1,0.7,0.1,0.8))
			#@ring.setColor(AGVector4.new(1,0.7,1,0.8))
		end
	end

		
	def getRing
		makeBigRingMesh
	end
	def setupMeshBoss
		setupMesh
		setupRing
	end

	def setupRing
		@ring=getRing
		if @selected
			#f6c108
			@ring.setRingColor(AGVector4.new(1,0.7,0.1,0.8))
		else
			@ring.setRingColor(AGVector4.new(0.7,0.7,1,0.8))
		end
		addMesh(@ring,AGVector3.new(0,0,0))
		#@ring.setVisible(false)
		updateRingColor
	end

	def eventHLJobFinished(job)
		puts "eventHLJobFinished(job) #{self}"
		getMap.eventHLJobFinished(self,job)
		doEvent(:eventHLJobFinished)
	end

	def checkHLJobEnd(man)
		if @job
			if man
				@job.check(man)
			end
			if @job.finished then 
				job=@job
				@job=nil 
				eventHLJobFinished(job)
				if @player
					@player.eventJobFinished(self,job)
				end
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

	def getFormation(man,pos=nil)
		if @formation.nil?
			getPos2D
		else
			return @formation.getPosition(man,pos)
		end
	end

	def eventHaveDefeated(e)
		if newHLJobs
			eventNoJob
			return
		end
		puts "#{getName} has defeated #{e.getName}"
		if @job.is_a?(AntHeroFightJob)
			@job.haveDefeated(e)
		end
	end
	def saveXML(node)
		super
		if @job
			n=node.addChild("hljob")
			n.set("type",@job.xmlName)
			@job.saveXML(n)
		end
	end

private
	def updateRingColor
		@ring.setVisible((@hovered or @selected))
		if @hovered and not @selected
			@ring.setRingColor(AGVector4.new(0.7,0.7,1,0.8))
		elsif @selected
			@ring.setRingColor(AGVector4.new(1,0.7,0.1,0.8))
		end
	end
end


