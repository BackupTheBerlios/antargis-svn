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

# try to implement alle entites in ruby
# WARNING: DON'T MEMBER_VARIABLES AS IT SEEMS TO CRASH RUBY SOMEHOW
# could be that it has something to do with Init_Stack ???

require 'ant_manbase.rb'



def genName
	nstart=["Ban","Gor","Bal","Da","Arg","Ol","Gar","Hek","Gav"]
	nend=["dor","in","and","or","tor","ain","yn"]
	nstart.shuffle[0]+nend.shuffle[0]
end


class AntNewMan<AntMyEntity
	include AntManBase

	attr_reader :meshState, :dead
	
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

		@weapon=["dagger","sword","shield"].shuffle[0]

	end
	
	##########################
	# EVENTS
	##########################
	def eventNoJob
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
		super
		setMeshState("fight")
	end
	
	def newFetchJob(p,target,r)
		super
		setGoAnim
	end
	
	def newMoveJob(p,target,n)
		super
		setGoAnim
	end
	
	def setGoAnim
		#dputs "SET STAND..."
		#getFirstMesh.setAnimation("go")
		setMeshState("walk")
		#dputs "OK"
	end
	
	def setStandAnim
		#dputs "SET STAND..."
		#getFirstMesh.setAnimation("stand")
		setMeshState("stand")
		#dputs "OK"
	end
	
	def setFighting(v)
		@fighting=v
	end

	def xmlName
		return "antNewMan"
	end

	def getWeapon
		@weapon
	end
	
	def setMeshState(name)
		if @meshState=="dead"
			puts "ERROR in setMeshState"
			puts name
		end
		#return
		#puts "setState #{name}"
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

				#setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_dagger.anim")))
				#setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_sword.anim")))
				#getFirstMesh.setAnimation("fight")
			when "fight"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_fight.anim")))
				if false
				case getWeapon
					when "dagger"
						setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_dagger.anim")))
					when "shield"
						setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_shield.anim")))
					when "sword"
						setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_e_shield.anim")))
				end
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
		end
		setDirection(dir)
	end

	def digResource(res)
		newRestJob(2+getRand)
		case res
			when "wood"
				setMeshState("axe")
			when "food"
				setMeshState("stand")
			else
				setMeshState("pick")
		end
	end

	def collectResource(res)
		case res	
			when "wood"
				setMeshState("wood")
			when "stone"
				setMeshState("stone")
		end
	end
	

	
	def saveXML(node)
		super(node)
		node.set("bossName",@bossName)
	end
	def loadXML(node)
		super(node)
		@bossName=node.get("bossName")
	end
	
	def animationEvent(name)
		case name
			when "hackaxe","pick","sword"
				playSound(name)
			when "ugh"
				#playSound(name)
		end
	end

	def eventMoraleLow
		if @boss
			setNoBoss
		end
	end

end

	