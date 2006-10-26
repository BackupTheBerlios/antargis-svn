#--
# Copyright (c) 2006 by David Kamphausen. All rights reserved.
#
# ant_ai_interface.rb
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
# == File overview
# This file contains all the modules and classes used for the scripting interface


#
# == BoA - AI Interface
# This file provides the complete interface, that should be used
# for implementing the AI
#
# It'd be best, if the human-player interface uses these interfaces, too (only).
#
#

# module AISensorInterface
# 
# 	def getPlayerName
# 		@player
# 	end
# 
# 	def getPlayerNames
# 		@map.players.collect{|p|p.getName}
# 	end
# 	def getOwnHeroes
# 		getHeroes(@player)
# 	end
# 
# 	def getHeroes(playerName)
# 		@map.getHeroes.select{|h|h.getPlayer and h.getPlayer.getName==playerName}.collect{|h|h.uid}
# 	end
# 	def getBuildings(playerName)
# 		@map.getBuildings.select{|h|g.getPlayer and h.getPlayer.getName==playerName}.collect{|h|h.uid}
# 	end
# 	def getVillages(playerName)
# 		@map.getVillages.select{|h|g.getPlayer and h.getPlayer.getName==playerName}.collect{|h|h.uid}
# 		# FIXME
# 	end
# 	def getBuildingsOfVillage(villageId)
# 		# FIXME
# 	end
# 
# 	def getHeroTroopCount(heroId)
# 		hs=@map.getHeroes.select{|h|h.uid==heroId}
# 		if hs.length==1
# 			return hs[0].getMen.length
# 		else
# 			return 0
# 		end
# 	end
# 
# 	def getResourceInfo(heroID,resourceName)
# 		if getOwnHeroes.member?(heroID)
# 			return @map.getByUID(heroID).resource.get(resourceName)
# 		end
# 		return 0
# 	end
# 
# 	def findResource(heroId,resourceName,atLeast=3)
# 		if getOwnHeroes.member?(heroId)
# 			hero=@map.getByUID(heroId)
# 		
# 			unit=getSource(hero,resourceName,atLeast)
# 			if unit
# 				return unit.uid
# 			end
# 		end
# 		return -1
# 	end
# 
# 	def isAppliableResourceNear(heroId,resourceName,far=10)
# 		atLeast=3
# 		if getOwnHeroes.member?(heroId)
# 			hero=@map.getByUID(heroId)
# 			return getSource(hero,resourceName,atLeast)
# 		end
# 		return nil
# 	end
# 
# 	def getPlaceOf(unitID)
# 		unit=@map.getByUID(unitID)
# 		if unit
# 			return unit.getPos2D
# 		end
# 	end
# 	private
# 	def getSource(hero,what,atleast,tries=5)
# 		while tries>0
# 			puts "search from #{hero} #{what} #{atleast}"
# 			e=getMap.getNext(hero,what,atleast)
# 			if e.nil?
# 				puts "NO SOURCE"
# 				#return nil
# 			elsif e.getPlayer and e.getPlayer.getName==@player
# 				return e
# 			end
# 			tries-=1
# 		end
# 		puts "OUT OF TRIES"
# 		return nil
# 	end
# 
# 
# end
# 
# module AIActionInterface
# 	def moveTo(heroId,place)
# 		if heroOk(heroID)
# 			getHero(heroID).newHLMoveJob(0,place,0)
# 		end
# 	end
# 
# 	def attack(heroId,targetId)
# 		if heroOk(heroId) and (not buildingOk(targetId))
# 			getHero(heroId).newHLFightJob(getHero(targetId))
# 		end
# 	end
# 	
# 	def recruit(heroId,targetId)
# 		if heroOk(heroId) and buildingOk(targetId)
# 			getHero(heroId).newHLRecruitJob(getHero(targetId))
# 		end
# 	end
# 
# 	def takeFood(heroId,targetId)
# 		puts "TAKING food...."
# buildingOk(targetId)	
# 	puts "---"
# 
# 		if heroOk(heroId) and buildingOk(targetId)
# 			puts "yay"
# 			getHero(heroId).newHLTakeFoodJob(getHero(targetId))
# 		end
# 		puts "boooooooooooaa"
# 		puts "#{heroOk(heroId)} and #{buildingOk(targetId)}"
# 	end
# 
# 	def takeWeapon(heroId,targetId)
# 		if heroOk(heroID) and buildingOk(targetId)
# 			getHero(heroID).newHLTakeWeaponJob(getHero(targetId))
# 		end
# 	end
# 
# 	def attackAnimal(heroId,targetId)
# 		if heroOk(heroID)
# 			getHero(heroID).newHLFightAnimalJob(getHero(targetId))
# 		end
# 	end
# 
# 	def build(heroId,place,type)
# 		if heroOk(heroID)
# 			getHero(heroID).newHLBuildJob(place,type)
# 		end
# 	end
# 
# 	def rest(heroId,time)
# 		if heroOk(heroId)
# 			getHero(heroId).newHLRestJob(time)
# 		end
# 	end
# 
# 	private
# 	def heroOk(heroId)
# 		@map.getHeroes.select{|h|h.getPlayer and h.getPlayer.getName==@player and h.uid==heroId}.length!=0
# 	end
# 	def buildingOk(houseId)
# 		@map.getBuildings.select{|h|h.getPlayer and h.getPlayer.getName==@player and h.uid==houseId}.length!=0
# 	end
# 	def getHero(heroId)
# 		@map.getByUID(heroId)
# 	end
# end
# 
# #
# # The scripting-interface that's used for scripting AI and levels. It's parted into an interface for actively accessing the world
# # or let's say your heroes and an interface for sensoring.
# # If you're writing new actions or sensors you'll be using these interfaces. The actual AI-Rules shouldn't use this interface anymore.
# class AIInterface
# 	include AIActionInterface
# 	include AISensorInterface
# 
# 	def initialize(map,player)
# 		@map=map
# 		@player=player
# 	end
# end
# 
# 
# class AIInterfaceTools<AIInterface
# 	def findNearestEnemy(heroId)
# 	end
# 
# 
# 	def getWeakestEnemyHouse(hero)
# 		bs=[]
# 		getPlayerNames.select{|n|n!=@player}.each{|p|
# 			bs+=getBuidlings(p)
# 		}
# 		hs=bs.sort{|a,b|a.getMen.length<=>b.getMen.length}
# 		hs[0]
# 	end
# 
# end

###############################################################################################################
###############################################################################################################
###############################################################################################################

# REWRITE

class AIInterface
	def initialize(map,player)
		@map=map
		@player=player
	end
	def myPlayer
		AIMyPlayer.new(@player)
	end
	def enemyPlayers
		@map.players.select{|p|p!=@player}.collect{|p|AIEnemyPlayer.new(p)}
	end
end

class AIMyPlayer
	def initialize(player)
		@player=player
	end
	def getHeroes
		@player.getHeroes.select{|p|p.is_a?(AntHero)}.collect{|p|AIMyHero.new(p,@player)}
	end
	def getBuildings
		@player.getHeroes.select{|p|p.is_a?(AntHouse)}.collect{|p|AIMyBuilding.new(p,@player)}
	end
	def getName
		@player.getName
	end
end

class AIMyEntity
	def initialize(ent,player,map)
		@ent=ent
		@player=player
		@map=map
	end
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
	def uid
		@ent.uid
	end
	def valid
		@ent.getPlayer==@player
	end
	def getName
		@ent.getName
	end
	def getPos
		@ent.getPos2D
	end
	private
	def getRef(t)
		@map.getByUID(t.uid)
	end
	def getSource(hero,what,atleast,tries=5)
		while tries>0
			puts "search from #{hero} #{what} #{atleast}"
			e=getMap.getNext(hero,what,atleast)
			if e.nil?
				puts "NO SOURCE"
				#return nil
			elsif e.getPlayer and e.getPlayer==@player
				if e.is_a?(AntHouse)
					return AIMyBuilding.new(e,@player,@map)
				elsif e.is_a?(AntHero)
					return AIMyHero.new(e,@player,@map)
				end
# 			else
# 				puts "#{e.getPlayer} #{@player}"
# 				raise 1
			end
			tries-=1
		end
		puts "OUT OF TRIES"
		return nil
	end
end

class AIMyBuilding<AIMyEntity
end

class AIMyHero<AIMyEntity
	# get the current count of troopers
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


	# set the aggression level (0,1,2)
	def setAggression(level)
		return unless [0,1,2].member?(level)
		if valid
			@ent.setAggression(level)
		end
	end

	# move my hero to place
	def moveTo(place)
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
end

class AIEnemyPlayer
	def initialize(player)
		@player=player
	end
	def getHeroes
		@player.getHeroes.select{|p|p.is_a?(AntHero)}.collect{|p|AIEnemyHero.new(p,@player)}
	end
	def getBuildings
		@player.getHeroes.select{|p|p.is_a?(AntHouse)}.collect{|p|AIEnemyBuilding.new(p,@player)}
	end
	def getName
		@player.getName
	end
end


class AIEnemyEntity
	def initialize(ent,player)
		@ent=ent
		@player=player
	end
	def getName
		@ent.getName
	end
	def getPos
		@ent.getPos2D
	end
	def uid
		@ent.uid
	end
	def valid
		@ent.getPlayer==@player
	end
end

class AIEnemy<AIEnemyEntity
end

class AIEnemyHero<AIEnemy
	def menCount
		@ent.getMen.length
	end
end

class AIEnemyBuilding<AIEnemy
	def menCount
		@ent.getMen.length
	end
end
