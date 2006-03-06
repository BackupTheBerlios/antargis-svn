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


class AntNewAngel<AntMyEntity

	def initialize
		super(AGVector2.new(0,0))
		setProvide("angel",true)
		@age=0
	end
	def move(time)
		p=getPos3D
		p.y+=time*20
		setPos(p)
		@age+=time
		if @age>10 then # 10 seconds old
			# discard
			getMap.removeEntity(self)
			getMap.endChange
		end
	end
	def getTexture
		return "angel.png"
	end
end

class AntNewMan<AntMyEntity
	attr_reader :meshState, :dead
	
	def initialize()
		super(AGVector2.new(0,0))
		setProvide("man",true)
		@signed=false
		@dead=false
		@fighting=false
		@bossName=""
		dputs "NEWMAN"
		setMeshState("walk")
		@mode="wait"
		setMinimapColor(AGColor.new(0x77,0x77,0x77))
	end
	
	def setMode(mode)
		@mode=mode
	end
	def getMode
		@mode
	end
	
	##########################
	# EVENTS
	##########################
	def eventNoJob
		eventJobFinished
	end
	
	def eventDefeated
		if @boss then
			@boss.eventManDefeated(self)
		end
	end
	
	def eventDie
		simDeath
		eventDefeated
		newRestJob(20)
		setMeshState("dead")
		if @boss
			@boss.removeMan(self)
		end
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
	
	##########################
	# setBoss
	##########################
	def setNoBoss()
		@bossName=""
	end
	
	def setBoss(hero)
		@bossName=hero.getName
		hero.signUp(self)
	end

	
	##########################
	# private
	##########################	
	def sendAngel
		e=AntNewAngel.new
		e.setPos(getPos2D)
		getMap.insertEntity(e)
		getMap.endChange
	end

	# simulate death:
	#   - transfer appearance to gravestone
	#   - start angel
	def simDeath
		if @dead
			return
			if @boss
				@boss.removeMan(self)
			end
			getMap.removeEntity(self)
			getMap.endChange
			return 
		else
			@dead=true
			#setMesh(Mesh.new(getMeshData("data/models/grave.ant2",1.0),AGVector4.new(0,0,0,0),40))
			#updateSurface
			sendAngel
		end
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
		#if vis
			setStandAnim
		#end
		#setVisible(vis)
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
	
	def setMeshState(name)
		#puts "setState #{name}"
		@meshState=name
		dir=getDirection
		case name
			when "wood"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_wood.anim")))
			when "stone"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_stones.anim")))
			when "walk","sitdown","sit"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_walk.anim")))
				getFirstMesh.setAnimation(name)
			when "fight"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_fight.anim")))
			when "axe"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_axe.anim")))
				#getSoundManager.playWave("data/sound/tree_chop.wav")
			when "pick"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_pick.anim")))
			when "stand"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_walk.anim")))
				getFirstMesh.setAnimation("stand")
			when "dead"
				setMesh(AnimMesh.new(getMap.getScene,getAnimMeshData("data/models/man_walk.anim")))
		 		setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/grave.ant2",0.2,"data/textures/models/grave.png"),AGVector4.new(0,0,0,0),0))
		end
		setDirection(dir)
	end
	
	def digResource(res)
		newRestJob(2)
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
	
	def sitDown
		newRestJob(0.4)
		setMeshState("sitdown")

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
			when "hackaxe","pick","sword","ugh"
				playSound(name)
		end
	end
end

	