#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_man.rb
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

require 'ant_manbase.rb'
require 'ant_config.rb'

def genName
	nstart=["Ban","Gor","Bal","Da","Arg","Ol","Gar","Hek","Gav"]
	nend=["dor","in","and","or","tor","ain","yn"]
	nstart.shuffle[0]+nend.shuffle[0]
end


class AntMan<AntRubyEntity
	include AntManBase

	attr_reader :meshState, :dead
	attr_accessor :target
	
	def initialize()
		super(AGVector2.new(0,0))
		puts "NEW MAN"
		setProvide("man",true)
		@signed=false
		@dead=false
		@fighting=false
		@bossName=""
		dputs "NEWMAN"
		setMeshState("walk")
		@mode="wait"
		setMinimapColor(AGColor.new(0x77,0x77,0x77))
		setName(genName)

		#resource.set("sword",1)
		checkResources
		self.age=(getRand*20+15)
		setHunger(0.006)
	end
	
	##########################
	# EVENTS
	##########################
	def eventNoJob
		super
		eventJobFinished
	end
	
	
	# Job was finished
	# 1) if no energy or dead: simDeath
	def eventJobFinished
		setVisible(true)
		super
		if getEnergy==0 or @dead then
			simDeath
			return
		end
		
		setFighting(false)
		
		if @bossName=="" then
		
			house=getMap.getNext(self,"house")
			if house
				houseName=house.getName
				if houseName=="" then
					dputs "ERROR House has no name!"
					exit
				end
				@bossName=houseName
				newRestJob(rand()*2)
				#house=getMap.getRuby(house)
				house.signUp(self)
				@signed=true
			end
		else
			boss=getMap.getByName(@bossName)
			if not boss
				@bossName=""
				return
			end
			@boss=boss
			if not @signed then
				dputs @bossName
				dputs boss
				boss.signUp(self)
				@signed=true
			end
			boss.assignJob(self)
		end
	end	

	def eventHaveDefeated(e)
		puts "#{getName} has defeated #{e.getName}"
		@boss.eventHaveDefeated(e)
	end

	def getPlayer
		if @boss
			@boss.getPlayer
		else
			nil
		end
	end

	##########################
	# setBoss
	##########################
	def setNoBoss()
		if @boss
			@boss.removeMan(self)
		end
		@bossName=""
		@boss=nil
	end
	
	def setBoss(hero)
		if @boss
			@boss.removeMan(self)
		end
		@bossName=hero.getName
		@boss=hero
		hero.signUp(self)
	end

	
	def checkHideAtHome
		if @boss
			if @boss.methods.member?("atHome") and @boss.class!=AntHero
				if @boss.atHome(self)
					return true
				end
			end
		end
		return false
	end
	
	def newRestJob(time)
		vis=checkHideAtHome
		setStandAnim
		super(time)
	end
	
	def newFightJob(d,ptarget)
		checkResources
		super
		@moving=true
		setMeshState("fight")
	end
	
	def newFetchJob(p,target,r)
		super
		setGoAnim
	end
	
	def newMoveJob(p,target,n)
		if isOnWater and isOnOpenWater(target) and (not haveBoat)
			newRestJob(2)
			return
		end

		super
		setGoAnim
	end
	
	def setGoAnim
		setMeshState("walk")
	end
	
	def setStandAnim
		setMeshState("stand")
	end
	
	def setFighting(v)
		@fighting=v
	end

	def getWeapon
		if resource.get("bow")>0
			return "bow"
		end
		if resource.get("sword")==0
			return "dagger"
		elsif resource.get("shield")==0
			"sword"
		else
			"shield"
		end
	end

	def setMeshState(name)
		if @meshState=="dead"
			puts "ERROR in setMeshState"
			puts name
		end
		@origMeshState=name
		name=checkOnWater(name)
		@meshState=name
		dir=getDirection
		case name
			when "wood"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_wood.anim")))
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_wood.anim")))
			when "stone"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_stones.anim")))
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_stones.anim")))
			when "walk","sitdown","sit"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_walk.anim")))
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_walk.anim")))
				getFirstMesh.setAnimation(name)
			when "fight"
				if @moving
					# FIXME: fill in man_e_dagger ...
					setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_walk.anim")))
					getFirstMesh.setAnimation("walk")
				else
					case getWeapon
						when "dagger"
							setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_dagger.anim")))
						when "shield"
							setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_shield.anim")))
						when "sword"
							setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_sword.anim")))
						when "bow"
							setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_bow.anim")))
					end
					#end
				end
			when "axe"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_axe.anim")))
				#getSoundManager.playWave("data/sound/tree_chop.wav")
			when "pick"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_pick.anim")))
			when "stand"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_walk.anim")))
				getFirstMesh.setAnimation("stand")
			when "dead"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_walk.anim")))
		 		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/grave.ant2",0.2,"data/textures/models/grave.png"),AGVector4.new(0,0,0,0),0))
			when "row"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_walk.anim")))
				getFirstMesh.setAnimation("sit")
				mesh=Mesh.new(getMap.getScene,getBoatMeshData,AGVector4.new(0,0,0),0)
				addMesh(mesh,AGVector3.new(0,0,0))
		end
		setDirection(dir)
	end


	def digResource(res)
		newRestJob(2+getRand)
		case res
			when "wood"
				setMeshState("axe")
			when "fruit"
				setMeshState("stand")
			else
				setMeshState("pick")
		end
	end

	def collectResource(res)
		case res	
			when "wood"
				setMeshState("wood")
			when "stone","ore","coal" # FIXME new animation for other resources
				setMeshState("stone")
		end
	end

	def eventStartMoving
		@moving=true
		setMeshState(@meshState)
		eventJobFinished # call, so that hl-job finish is checked
	end
	def eventStartFighting
		@moving=false
		setMeshState(@meshState)
	end
	
	def canCarry
		3
	end
	
	def saveXML(node)
		super(node)
		node.set("bossName",@bossName)
	end
	def loadXML(node)
		super(node)
		@bossName=node.get("bossName")
		if getMap.getPos(getPos2D).z>0
			setOnWater(false)
		end
	end
	
	def animationEvent(name)
		case name
			when "hackaxe","pick","sword"
				playSound(name)
			when "ugh"
				playSound(name)
			else
				super
				#playSound(name)
		end
	end

	def eventMoraleLow
		if @boss
			setNoBoss
		end
	end
	def checkResources
		if resource.get("bow")>0
			setStrength(0.03)
		elsif resource.get("sword")>0
			setStrength(0.02)
		else
			setStrength(0.012)
		end
		if resource.get("shield")==0
			setDefense(1)
		else
			setDefense(1.5)
		end
	end
	def getDescription
		if @boss.nil?
			bname=_("nobody")
		else
			bname=@boss.getName
		end
		_("This is {1}. He is {2} years old.",getName,age)+_("He obeys {1}.",bname)
	end
private
end

	