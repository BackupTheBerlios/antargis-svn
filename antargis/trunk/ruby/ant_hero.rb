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
require 'ant_manbase.rb'

class AntHero<AntBoss
	include AntManBase

	attr_reader :meshState, :dead
	def initialize
		super
		@men.push(self)
		setProvide("hero",true)
		@appearance="hero"
		getMap.setLight(self)
		setMinimapColor(AGColor.new(0xFF,0,0))
		@meshStand="stand"
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
		@portrait=node.get("portrait")
	end
	
	def noHLJob
		puts "noHLJob"
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
		assignJob2All
	end
	
	def assignJob(man)
		checkHLJobEnd(man)
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
	def newHLTakeFoodJob(target)
		@job=AntHeroTakeFoodJob.new(self,target,$buttonPanel.getAggression)
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
		getMap.eventHLDismissed(self)
	end
	
		
	# formation:
	# 1) wait for 3/4 of people are in formation but max. 5 seconds or so
	# 2) start all at once
	
	def getSitFormation(man)
		if man==self
			return getPos2D
		end
		if not @men.member?(man)
			raise "not my man! : #{man} self:#{self}"
		end
		id=@men.index(man)-1  # first index is hero himself
		
		if id then
			angle=id.to_f/(@men.length-1)*Math::PI*2
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
		if man==self
			return AGVector2.new(0,0)
		end
		id=@men.index(man)-1  # first index is hero himself
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
		puts "eventAttacked #{by}"
		newHLDefendJob(by)
	end
	
	def assignJob2All
		super
		dputs "ASSIGNJOB"
		setFire(false)
	end

	def setMeshState(name)
		puts "FIXME: implement setMeshState(.)"
		@meshState=name
		dir=getDirection
		case name
			when "dead"
		 		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/grave.ant2",0.2,"data/textures/models/grave3.png"),AGVector4.new(0,0,0,0),0))
			else
		 		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/hero_lp.ant2",0.08,"data/textures/models/hero_lp.png"),AGVector4.new(0,0,0,0),0))
		end
		setDirection(dir)
	end



 	def setupMesh
		setMeshState("normal")
	end
	def getRing
		makeRingMesh
	end

	def getImage
		@portrait||=""
		r=nil
		if @portrait.length!=0
			puts "port!=0"
			r=getTextureCache.get(@portrait)
		else
			puts "port==0"
			r=getTextureCache.get("data/gui/portraits/#{getName}.png")
		end
		puts "getImage-ok"
		return r
	end
	def getDescription
		"This is #{getName}. He is #{age} years old. #{getMen.length} men support him."
	end
	def age
		21
	end

	def spreadResources
		# FIXME
	end

end

