#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_wolf.rb
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

# Note:
# This implementation is still very buggy. Apart from this there are some things missing:
# * animation
# * move wolves around a little while resting
# * let them strive around when not hungry
# * attack people if no sheep near

# Wolves are controlled in a pack by a PackLeader.
# Typically wolves are resting. When a member gets too much hunger (<0.5), then
# the whole pack moves on to find some food (sheep or men)

class AntWolf<AntAnimal
	attr_accessor :leader,:mypack

	def initialize()
		super(AGVector2.new(0,0))
		setProvide("wolf",true)
		setProvide("wolf_leader",true)
		setSpeed 3
		@lastBirth=0
		@foodAdd=0
		@mypack=[]
		@leader=nil
		
		setMesh

		resource.set("food",1)
		@job=:resting
		setHunger(0.01)
		setMode("resting")
	end
	def eventNoJob
		eventJobFinished
	end
	def eventJobFinished
		super

		if @dead
			newRestJob(30)
			if @alreadyDead
				getMap.removeEntity(self)
			end
			@alreadyDead=true
	
			return
		end

		if getFood<=0
			eventDie
			return
		end

		# BIRTHRATE is here:
		if @lastBirth>40 then
			# make child
			puts "A WOLF IS BORN"
			wolf=AntWolf.new
			wolf.setPos(getPos2D)
			getMap.insertEntity(wolf)
			#getMap.endChange
			newRestJob(2)
			@lastBirth=-getRand*10
		else
			checkPackMerge
			if @leader
				assignJob(self)
			else
				decideWhatsToDo
			end
		end
# 
# 		elsif rand<0.5 then
# 			newMoveJob(0,getTargetPos,0)
# 			setMeshState("go")
# 		else
# 			newRestJob(3)
# 			setMeshState("eat")
# 			playSound("wolf")
# 		end

		@foodAdd+=1
		#puts "FOOOOOD #{@foodAdd}"
		if @foodAdd>3 and resource.get("food")<10
			#puts "RESOURCE: #{resource.get("food")}"
			resource.add("food",1)
			#puts "RESOURCE: #{resource.get("food")}"
			@foodAdd=0
		end

		@lastBirth+=1
	end

	def checkPackMerge
		# find next leader
		setProvide("wolf_leader",false)

		tent=getMap.getNext(self,"wolf_leader")
		if tent
			if (tent.getPos2D-getPos2D).length<10
				if not tent.inPack(self)
					# simple take me as leader
					tent.setLeader(self)
				end
			end
		end
	end

	def assignLeader(pack)
		oldpack=@mypack.dup
		@mypack+=pack
		@mypack.uniq!
		@leader=nil
		@mypack.delete(self)
		if @mypack.length!=oldpack.length
			@mypack.each{|w|w.setLeaderDirect(self)}
		end
		setProvide("wolf_leader",true)
	end

	def inPack(wolf)
		@mypack.member?(wolf)
	end
		

	def setLeader(leader)
		leader.assignLeader(@mypack+[self])
		setLeaderDirect(leader)
	end

	def setLeaderDirect(leader)
		@mypack=[]
		@leader=leader
		setProvide("wolf_leader",false)
	end
	
	def decideWhatsToDo
		if @job==:resting
			if getFood>0.5
				@job=:resting
			else
				puts "GETTING NEW SHEEP"
				@sheep=getMap.getNext(self,"sheep")
				if @sheep
					@job=:killsheep
				else
					@job=:resting
				end
			end
		
			assignJobToAll
		else
			assignJob(self)
		end
	end

	def assignJobToAll
		all=@mypack+[self]
		all.each{|w|assignJob(w)}
	end

	def assignJob(wolf)
		case @job
			when :resting
				wolf.newRestJob(4)
				wolf.setMode("resting")
			when :killsheep
				if @sheep
					puts "KILLSHEEP #{@sheep} #{self}"
					if (@sheep.getPos2D-wolf.getPos2D).length>0.5
						wolf.newMoveJob(0,@sheep.getPos2D,0)
					else
						@sheep.eventDie	
						if @leader
							@leader.shareFood
						else
							shareFood
						end
					end
				end
		end
	end

	def shareFood
		([self]+@mypack).each{|w|w.incFood(1.5)}
		@job=:resting
		decideWhatsToDo
	end
	
	def getTargetPos
		p=getPos2D
		t=p
		ok=true
		radius=4
		# assure that sheep doesn't walk into water
		while getMap.getHeight(t.x,t.y)<0.1 or ok do
			d=AGVector2.new(getRand-0.5,getRand-0.5).normalized*radius
			t=p+d
			t=getMap.truncPos(t)
			ok=false
		end 
		return t
	end
	def getMen
		(@mypack+[self]).uniq.dup
	end
	def menCount
		getMen.length
	end
	def setMeshState(s)
		#getFirstMesh.setAnimation(s)
	end
	def getName
		return "Corpse" if @dead
		return "Wolfleader2" if provides("wolf_leader")
		return "Wolfleader" if @leader.nil?
		"Wolf"
	end
	def eventDie
		setMesh(:rip)
		#setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/rip.ant2",0.3,"data/textures/models/rip.png"),AGVector4.new(0,0,0,0),0))
		@dead=true
		
		newRestJob(1)

		if @leader.nil?
			# choose new leader
			if @mypack.length>0
				l=@mypack[0]
				l.assignLeader(@mypack[1..-1])
				@mypack=[]
			end
		end
	end
end
