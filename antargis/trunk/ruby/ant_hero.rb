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
		self.age=(getRand*20+20)
		setHunger(0.006)
	end
	def setAppearance(a)
		@appearance=a
	end
	
	def saveXML(node)
		super(node)
		node.set("appearance",@appearance.to_s)
	end
	def loadXML(node)
		super(node)
		@appearance=node.get("appearance")
		if @appearance==""
			@appearance="hero"
		end
		@portrait=node.get("portrait")

		if node.get("primary")=="true"
			#setMoraleHealSpeed=1000
			setMoraleFlag(false)
		end
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
			@fireSound=playLoopSoundGlobal(self,"fire",getPos2D,0.4)
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
			stopLoopSound(self)
			#getSoundManager.stopChannel(@fireSound)
			@fireSound=nil
		end
	end
	
	def newHLRestJob(time)
		@job=nil
		setFire(true)
		super(time)
		assignJob2All
	end
	
	def assignJob(man)
		checkHLJobEnd(man)
	end	
	def moveHome(man)
		if @formation.nil?
			raise "formation is nil!"
		end
		pos=@formation.getPosition(man)
# 		pos=getSitFormation(man)
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
	def newHLConstructJob(target)
		@job=AntHeroConstructJob.new(self,target,$buttonPanel.getAggression)
		assignJob2All
	end
	def newHLTakeFoodJob(target)
		@job=AntHeroTakeJob.new(self,target,$buttonPanel.getAggression,"food")
		assignJob2All
	end
	def newHLTakeWeaponJob(target)
		@job=AntHeroTakeJob.new(self,target,$buttonPanel.getAggression,"weapon")
		assignJob2All
	end
	def newHLFightJob(target)
		@job=AntHeroFightJob.new(self,target)
		assignJob2All
	end
	def newHLFightAnimalJob(target)
		@job=AntHeroFightAnimalJob.new(self,target)
		assignJob2All
	end
	def newHLBuildJob(pos,type)
		@job=AntHeroBuildJob.new(self,pos,type)
		assignJob2All
	end
	
	def newHLDismissJob()
		@job=nil
		agg=$buttonPanel.getAggression
		men=@men.select{|m|not m.is_a?(AntHero)} # exclude hero
		c=(men.length-1)*agg/3
			
		men=men[0..c]
		men.each{|m|
			m.setNoBoss
			@men.delete(m)
			m.delJob
		}
		getMap.eventHLDismissed(self)
	end
	
	def upgradeMan(man)
		# copy properties through xml-node	
		xmlnode=Node.new("test")
		man.saveXML(xmlnode)
		
		nhero=AntHero.new
		nhero.loadXML(xmlnode)
		removeMan(man)
		getMap.removeEntity(man)
		getMap.insertEntity(nhero)
		#getMap.endChange # commit
		nhero.newRestJob(0)
		newHLRestJob(1)
		#nhero.setName("Sir "+nhero.getName)
		man.setName("xxxxxxxx") # reset name - so there's some distinction
		nhero.setOwner(self)
		nhero.experience=0
	end
	
	
	def setFire(flag)
		puts "setFire #{flag}"
		if flag
			if getPos3D.z>0 # won't start fire in water!!
				if not @fire
					@fire=AntFire.new(getPos3D+AGVector3.new(0.7,0,0))
					getMap.insertEntity(@fire)
				end
				startFireSound
			end
		else
			#raise 1
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
		if @job.class!=AntHeroRestJob
			setFire(false)
		end
		doEvent(:newJobAssigned)
	end

	def setMeshState(name)

		if name=="row"
			name="walk"
		end

		@origMeshState=name
		name=checkOnWater(name)
		puts "FIXME: implement setMeshState(.)"
		@meshState=name
		dir=getDirection
		case name
			when "row"
		 		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/hero_lp.ant2",0.08,"data/textures/models/hero_lp.png"),AGVector4.new(0,0,0,0),0))
				#setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_walk.anim")))
				#getFirstMesh.setAnimation("sit")
				mesh=Mesh.new(getMap.getScene,getBoatMeshData,AGVector4.new(0,0,0),0)
				addMesh(mesh,AGVector3.new(0,0,0))

			when "dead"
		 		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/grave.ant2",0.2,"data/textures/models/grave3.png"),AGVector4.new(0,0,0,0),0))
			else
		 		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/hero_lp.ant2",0.08,"data/textures/models/hero_lp.png"),AGVector4.new(0,0,0,0),0))
		end
		setDirection(dir)
		setupRing

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

		filename=""
		if @portrait.length!=0
			puts "port!=0"
			filename=@portrait
			#r=getTextureCache.get(@portrait)
		else
			puts "port==0"
			#r=getTextureCache.get("data/gui/portraits/#{getName}.png")
			filename="data/gui/portraits/#{getName}.png"
		end
		if fileExists(filename)
			r=getTextureCache.get(filename)
		else
			r=getTextureCache.get("data/gui/portraits/none.png")
		end

		puts "getImage-ok"
		return r
	end
	def getDescription
		nameage=_("This is {1}. He is {2} years old.",getName,age)
		support=nil
		if getMen.length==1
			support=_("Nobody supports him.")
		else
			support=_("{1} men support him.",getMen.length-1)
		end
		nameage+" "+support
	end

	def spreadResources
		# FIXME
	end

	def eventDie
		super
		@job=nil
		if @player
			@player.remove(self)
		end
		getMap.eventHeroDied(self)
	end

end

