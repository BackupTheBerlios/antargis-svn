#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hljobs.rb
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


# AntAngle is an entity for displaying angels flying up
# when an AntMan dies. This is currently not supported, because
# there is no AntAngel mesh.
class AntAngel<AntRubyEntity

	def initialize(map)
		super
		setProvide("angel",true)
		@age=0
	end
	# simply move mesh up
	# FIXME:
	# this is very slow - should be removed and replaced by a MoveJob(3d)
	def move(time)
		p=getPos3D
		p.y+=time*20
		setPos(p)
		@age+=time
		if @age>10 then # 10 seconds old
			# discard
			getMap.removeEntity(self)
			#getMap.endChange
		end
	end
end

# AntManBase is a base-class/module for all men (including heroes)
# it contains functionality for:
# * rowing a boat
# * sitting down
# * dying (and sending an angel)
# * shooting arrows
module AntManBase

	# call this to make a man sitdown
	# * animation is played
	# * setMeshState is needed for this
	def sitDown
		newRestJob(0.4)
		setMeshState("sitdown")
	end

	def lookTo(p)
		setDirection(180-(p-getPos2D).normalized.getAngle.angle*180.0/Math::PI)
	end

	def walkTo(p)
		p=p.getPos2D if p.is_a?(AntEntity)
		newMoveJob(0,p,0)
	end
	
	def standStill
		newRestJob(2)
		setMeshState("stand")
	end

	def standStillShort
		newRestJob(0.1)
		setMeshState("stand")
	end

	def sitStill
		checkEat
		newRestJob(2)
		setMeshState("sit")
	end


	# overrides newFightJob from AntEntity
	# fighting distance is assigned 1 or 10 
	# depending on the man having a bow
	def newFightJob(p,target)
		if resource.get("bow")>0
			super(p,target,10)
		else
			super(p,target,1)
		end
	end

	def eventDefeated
		# FIXME: check, if this ok !!!! /HLJobs
		if newHLJobs
			hlJobMode[:defeated]=true
			eventNoJob
			return
		end


		if is_a?(AntBoss)
			eventManDefeated(self)
		elsif @boss then
			@boss.eventManDefeated(self)
		end
	end
	def eventNoJob
		if @meshState=="dead"
			if @mdead
				getMap.removeEntity(self)
				return
			else
				newRestJob(20)
				@mdead=true
				return
			end
		end
		super
	end

	def eventDie
		super
		simDeath
	end


	# simulate death:
	#   - transfer appearance to gravestone
	#   - start angel
	def simDeath
		playSound("die")
		if @boss
			@boss.removeMan(self)
		end

		# add grave
		grave=AntGrave.new(getMap)
		grave.type=:hero if self.is_a?(AntHero)
		grave.setPos(getPos2D)
		getMap.insertEntity(grave)

		# remove myself
		getMap.removeEntity(self)

		# add sack if resources not empty
		if not self.resource.empty
			sack=AntSack.new(getMap)
			sack.setPos(getPos2D+AGVector2.new(0.3,-0.3))
			getMap.insertEntity(sack)
			sack.resource.takeAll(self.resource)
			sack.resourceChanged
		end

	end


	def shouldFight
		canFight
	end

	def checkOnWater(name)
		z=getMap.getPos(getPos2D).z
		if isOnWater
			# under water
			if name=~/sit/ and not isOnOpenWater
				name="stand"
				setOnWater(false)
			end
			if isOnOpenWater and haveBoat
				puts "onOpenWater"
				setOnWater(true)
				name="row"
			end
		else
			setOnWater(false)
		end
		name
	end

	def checkResources
		# FIXME: maybe make hero a little stronger ???
		#        or even make experienced men stronger ???
		if resource.get("bow")>0
			setStrength(0.03)
			setMoraleStrength(0.04)
		elsif resource.get("sword")>0
			setStrength(0.024)
			setMoraleStrength(0.03)
		else
			setStrength(0.015)
			setMoraleStrength(0.02)
		end
		if resource.get("shield")==0
			setDefense(1)
		else
			setDefense(1.5)
		end
	end

	def eventHitWaterMark(fromAbove)
		#raise 1
		log "eventHitWaterMark(#{fromAbove})"
		#setOnWater(fromAbove)
		if fromAbove
			if haveBoat
				setMeshState("row")
				return true
			else
				# stop job
				delJob
				p=getMap.getNextPlaceAbove(getPos2D,-0.2)
				newMoveJob(0,p,0)
				delJob
				return false
			end
		else
			setOnWater(false)
			setPos(getMap.getPos(getPos2D))
			setMeshState(@origMeshState)
			return true
		end
	end

	def animationEvent(name)
		case name
			when "bow"
				arrow=AntArrow.new(getMap)
				arrow.setPos(getPos3D+AGVector3.new(0,0,1))
				pos=@fightTarget.getPos3D+AGVector3.new(0,0,1)
				arrow.newMoveJob(0,pos,0) #@target.getPos2D,0)
				getMap.insertEntity(arrow)
		end
	end
	def checkEat
		log "CHECKEAT"
		if getFood<0.5
			if resource.get("food")>0
				incFood(1)
				resource.sub("food",1)
			end
		end
	end

protected
	def haveBoat
		resource.get("boat")>=1
	end

private
	def sendAngel
		# FIXME: create mesh for angels ??
		return # do not send angels - they don't have meshes anyway
	end
end