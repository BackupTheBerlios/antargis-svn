#--
# Copyright (c) 2006 by David Kamphausen. All rights reserved.
#
# ant_level.rb
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
#++
#
# == Global Scripting-Interface Overview
# 

class AntScriptingJob
	def initialize(job)
		@job=job
	end
	def is_a?(pClass)
		return unless pClass.is_a?(Class)
		@job.is_a?(pClass)
	end
	def target
		if @job.respond_to?(:target)
			AntLevelEntity.new(@job.target)
		end
	end
end

module AntScriptingEntityFullAccess
	def resources(name=nil)
		return unless name.is_a?(String) or name.nil?
		if valid
			if name.nil?
				return @ent.resource.getAll
			else
				return @ent.resource.get(name)
			end
		else
			return {}
		end
	end
	def getName
		@ent.getName
	end
	def getPos
		@ent.getPos2D
	end
end

module AntScriptingHeroFullAccess
	include AntScriptingEntityFullAccess
	def menCount
		@ent.getMen.length
	end

	def findNearResource(resourceName,far=10)
		return unless resourceName.is_a?(String) and far.is_a?(Numeric)
		atLeast=3
		atLeast=0
		if valid
			return getSource(@ent,resourceName,atLeast)
		end
		return nil
	end

	def getJob
		AntScriptingJob.new(@ent.getJob)
	end


	# set the aggression level (0,1,2)
	def setAggression(level)
		return unless [1,2,3].member?(level)
		if valid
			@ent.setAggression(level)
		end
	end

	# move my hero to place
	def moveTo(place)
		dputs "MOVETO:",place,place.class
		place=place.pos if place.is_a?(TargetPos)
		place=place.dim2 if place.is_a?(AGVector3)
		dputs "MOVETO:",place,place.class
		return unless place.is_a?(AGVector2)
		if valid
			@ent.newHLMoveJob(0,place,0)
		end
	end

	# attack enemy target
	def attack(target)
		return unless target.is_a?(AIEnemy)
		if valid and target.valid
			r=getRef(target)
			if r
				@ent.newHLFightJob(r)
			end
		end
	end

	def dismiss
		@ent.newHLDismissJob
	end
	
	# recruit from target
	def recruit(target)
		return unless target.is_a?(AIMyBuilding)
		if valid and target.valid
			r=getRef(target)
			@ent.newHLRecruitJob(r) if r
		end
	end

	def takeFood(target)
		puts "takeFood: #{target}"
		return unless target.is_a?(AIMyBuilding)
		if valid and target.valid
			r=getRef(target)
			@ent.newHLTakeFoodJob(r) if r
		end
	end

	def takeWeapon(target)
		return unless target.is_a?(AIMyBuilding)
		if valid and target.valid
			r=getRef(target)
			if r
				@ent.newHLTakeWeaponJob(r)
			end
		end
	end

	def attackAnimal(target)
		return unless target.is_a?(AIAnimal)
		if valid
			r=getRef(target)
			if r
				@ent.newHLFightAnimalJob(r)
			end
		end
	end

	def build(place,type)
		return unless place.is_a?(AGVector2) and type.is_a?(String)
		if valid
			ds=getDescendantsOfClass(AntHouse)
			@map={}
			ds.each{|d|
				c=getChild(d.to_s)
				if c
					addHandler(c,:sigClick,:eventBuild)
					@map[d.to_s]=d
				end
			}
			type=@map[type]
			if type
				@ent.newHLBuildJob(place,type)
			end
		end
	end

	def rest(time)
		return unless time.is_a?(Numeric)
		if valid and time>0
			@ent.newHLRestJob(time)
		end
	end

	def addHandler(eventName,&block)
		@ent.addHandler(eventName,&block)
	end

end

module AntScriptingEntityEnhancedAccess
	# hacking
	def setPlayer(player)
		return nil if not player.is_a?(AntLevelPlayer)
		p=getMap.players.select{|p|p.getName==player.getName}[0]
		if p
			@ent.setPlayer(p)
		end
	end

	def setBoss(entity)
		return unless entity.is_a?(AntLevelEntity)
		@ent.setBoss(getMap.getByName(entity.getName))
	end

# 	def getMen
# 		@ent.getMen.collect{|e|AntLevelEntity.new(e)}
# 	end
end