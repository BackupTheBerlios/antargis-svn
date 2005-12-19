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
require 'ant_fire.rb'
require 'ant_ring.rb'

class AntHero<AntBoss
	def initialize
		super
		setType("hero")
		@appearance="hero"
		getMap.setLight(self)
#		setMyMesh
	end
	def setAppearance(a)
		@appearance=a
	end
	
	def xmlName
		return "antHero"
	end
	def saveXML(node)
		super(node)
		node.set("appearance",@appearance)
	end
	def loadXML(node)
		super(node)
		@appearance=node.get("appearance")
		if @appearance==""
			@appearance="hero"
		end
	end
	
	def getEnergy
		if @job and @job.class==AntHeroFightJob
			return @job.getEnergy
		else
			return 0
		end
	end
	
	def checkHLJobEnd(man)
		if @job
			if man
				@job.check(man)
			end
			if @job.finished then 
				if @player
					@player.eventJobFinished(self,@job)
				end
				@job=nil 
			end
		end
	end
	
	def noHLJob
		if @player
			@player.assignJob(self)
			#stopFireSound
		else
			# no player , so simply rest - to infinity (or at least 5 seconds)
			newHLRestJob(5)
		end
	end
	
	def startFireSound
		if not @fireSound
			dputs "STARTING FIRE"
			@fireSound=getSoundManager.loopPlay("data/sound/fire.wav",0.4)
			dputs @fireSound
		end
	end	
	def stopFireSound
		if @fireSound
			dputs "STOPPED"
			dputs @job
			if @job.class==AntHeroRestJob
				#raise "bla"
			end
			getSoundManager.stopChannel(@fireSound)
			@fireSound=nil
		end
	end
	
	def newHLRestJob(time)
		setFire(true)
		super(time)
	end
	
	def assignJob(man)
		if @fighting then
			checkFight
		elsif @job == nil or @job.class==AntHeroRestJob then
			# rest job
			formationPos=getSitFormation(man)
			if (man.getPos2D-formationPos).length2<0.2 then
				if not ["sitdown","sit"].member?(man.meshState)
					man.sitDown
				else
					man.newRestJob(5)
					man.setMeshState("sit")
				end
			else
				man.newMoveJob(0,formationPos,0)
			end
		else
			checkHLJobEnd(man)
		end
	end	
	def moveHome(man)	
		pos=getSitFormation(man)
		if (man.getPos2D-pos).length>1
			man.newMoveJob(0,pos,0)
		else
			man.newRestJob(2)
		end
	end
	
	
	def newHLMoveJob(prio,pos,dist)
		@job=AntHeroMoveJob.new(self,prio,pos,dist)
		assignJob2All
	end
	def newHLRecruitJob(target)
		@job=AntHeroRecruitJob.new(self,target,$buttonPanel.getAggression)
		assignJob2All
	end
	def newHLFightJob(target)
		@job=AntHeroFightJob.new(self,target)
		assignJob2All
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
			radius=1
			return AGVector2.new(Math::sin(angle)*radius,Math::cos(angle)*radius)+getPos2D
		else
			dputs "ERROR in SitFormation!"
			dputs "MEN:"+@men.to_s
			dputs "man:"+man.to_s
			dputs man.class
			raise "sitting error"
		end
	end
	
	def getWalkFormation(man,dir)
		id=@men.index(man)
		if id
			lineWidth=0.7
			if @men.length>30
				lineWidth=1.5
			end
		
			if id>=2 then
				id+=1 # exclude hero's position
			end
			line=id/5
			col=id%5
			col=col-2
			normal=dir.normal
			l=dir*line*lineWidth
			c=normal*col*0.5
			
			return l+c
		else
			dputs @men.length
			dputs man
			dputs "ERROR in WalkFormation!"
			exit
		end
	end
	
	
	def setFire(flag)
		if flag
			if not @fire
				@fire=AntFire.new(getPos3D+AGVector3.new(0.7,0,0))
				getMap.insertEntity(@fire)
			end
			startFireSound
		else
			if @fire
				@fire.disable
				@fire=false
			end
			stopFireSound
		end
	end
	
	def eventAttacked(by)
		newHLDefendJob(by)
	end
	
	def assignJob2All
		super
		dputs "ASSIGNJOB"
		setFire(false)
	end
# 
# 	def selected=(s)
# 		@selected=s
# 		setMyMesh
# 	end
# 	
# 	def hovered=(s)
# 		@hovered=s
# 		@hoverStart=$systemTime
# 		setMyMesh
# 	end
# 		
# 	def setMyMesh
# 		setMesh(Mesh.new(getMeshData("data/models/hero.ant",0.7),AGVector4.new(0,0,0,0),0))
# 		if @selected or @hovered
# 			@ring=makeRingMesh
# 			if @selected
# 				@ring.setColor(AGVector4.new(1,0.7,1,0.8))
# 			else
# 				@ring.setColor(AGVector4.new(0.7,0.7,1,0.8))
# 			end
# 			addMesh(@ring,AGVector4.new(0,0,0,0))
# 		end
# 	end
 	def setupMesh
 		setMesh(Mesh.new(getMeshData("data/models/hero.ant",0.7),AGVector4.new(0,0,0,0),0))
	end
	def getRing
		makeRingMesh
	end
end

