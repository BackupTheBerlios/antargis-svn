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

	attr_accessor :hlJobMode
	
	def initialize()
		super(AGVector2.new(0,0))

		@hlJobMode={}

		setProvide("man",true)
		@signed=false
		@dead=false
		@fighting=false
		@bossName=""
		@boss=nil
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
		
		if @boss.nil?
			if @bossName=="" then
			
				house=getMap.getNext(self,"house")
				if house
					houseName=house.getName
					if houseName=="" then
						dputs "ERROR House has no name!"
						exit
					end
					@bossName=houseName
					newRestJob(getRand*2)
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
			end
			puts "BOSS:#{@boss}"
		end
		if @boss and not @signed then
			@boss.signUp(self)
			@signed=true
		end
		if @boss
			@boss.assignJob(self)
		end
	end	

	def eventHaveDefeated(e)
		if newHLJobs
			eventNoJob
		else
			puts "#{getName} has defeated #{e.getName}"
			@boss.eventHaveDefeated(e)
		end
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
		puts "SETBOSS:#{hero}"
		if @boss
			@boss.removeMan(self)
			@boss=nil
		end
		if hero.nil?
			setNoBoss
			return	
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
		# FIXME: get MeshState somehow from job and resource-contents ???
		if name=="row"
			name="walk"
		end


		if @meshState=="dead"
			puts "ERROR in setMeshState"
			puts name
		end
		@origMeshState=name
		name=checkOnWater(name)
		@meshState=name
		dir=getDirection
		case name
			when "fight"
				if @moving
					setMesh("walk")
					getFirstMesh.setAnimation("walk")
				else
					#	raise 1
					puts "name:#{name}"
					assert{["dagger","shield","sword","bow"].member?(getWeapon)}
					setMesh("fight_"+getWeapon)
				end
			when "dead"
				setMesh(:grave)
			when "row"
				mesh=setMesh("sit")
				mesh.setAnimation("sit")
				addMesh(createModel(:boat),AGVector3.new(0,0,0))
			when "stand","axe","pick","wood","stone","flour","corn","walk","sitdown","sit"
				setMesh(name)
				if ["stand","walk","sitdown","sit"].member?(name)
					getFirstMesh.setAnimation(name)
				end
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
			when "stone","ore","coal","food","corn","crop","flour" # FIXME new animation for other resources
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

	def getHero
		@boss
	end
	
	def saveXML(node)
		super(node)
		node.set("bossName",@bossName)
		if @target
			node.set("targetEntity",@target.uid.to_s)
		end
	end
	def preloadXML(node)
		super
		@bossName=node.get("bossName")
	end

	def loadXML(node)
		super(node)
		if @bossName!=""
			boss=getMap.getByName(@bossName)
			if boss
				@boss=boss
				@boss.signUp(self)
				@signed=true
			end
		end

		if getMap.getPos(getPos2D).z>0
			setOnWater(false)
		end
		if node.get("targetEntity")!=""
			@target=getMap.getByUID(node.get("targetEntity").to_i)
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

	