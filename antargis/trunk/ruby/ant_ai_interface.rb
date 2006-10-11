#
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
#

#
# BoA - AI Interface
# ---------------------------------------
# This file provides the complete interface, that should be used
# for implementing the AI
#
# It'd be best, if the human-player interface uses these interfaces, too (only).
#
#

module AISensorInterface

	def getPlayerName
		@player
	end

	def getPlayerNames
		@map.getPlayers.collect{|p|p.getName}
	end
	def getOwnHeroes
		getHeroes(@player)
	end

	def getHeroes(playerName)
		@map.getHeroes.select{|h|h.getPlayer and h.getPlayer.getName==playerName}.collect{|h|h.uid}
	end
	def getBuildings(playerName)
		@map.getBuildings.select{|h|g.getPlayer and h.getPlayer.getName==playerName}.collect{|h|h.uid}
	end

	def getHeroTroopCount(heroId)
		hs=@map.getHeroes.select{|h|h.uid==heroID}
		if hs.length==1
			return hs[0].getMen.length
		else
			return 0
		end
	end

	def getResourceInfo(heroID,resourceName)
		if getOwnHeroes.member?(heroID)
			return @map.getByUID(heroID).resource.get(resourceName)
		end
		return 0
	end

	def findResource(heroId,resourceName,atLeast=3)
		if getOwnHeroes.member?(heroId)
			hero=@map.getByUID(heroId)
		
			unit=getSource(hero,resourceName,atLeast)
			if unit
				return unit.uid
			end
		end
		return -1
	end

	def isAppliableResourceNear(heroId,resourceName,far=10)
		atLeast=3
		if getOwnHeroes.member?(heroId)
			hero=@map.getByUID(heroId)
			return getSource(hero,resourceName,atLeast)
		end
		return nil
	end

	def getPlaceOf(unitID)
		unit=@map.getByUID(unitID)
		if unit
			return unit.getPos2D
		end
	end
	private
	def getSource(hero,what,atleast,tries=5)
		while tries>0
			puts "search from #{hero} #{what} #{atleast}"
			e=getMap.getNext(hero,what,atleast)
			if e.nil?
				puts "NO SOURCE"
				#return nil
			elsif e.getPlayer and e.getPlayer.getName==@player
				return e
			end
			tries-=1
		end
		puts "OUT OF TRIES"
		return nil
	end


end

module AIActionInterface
	def moveTo(heroId,place)
		if heroOk(heroID)
			getHero(heroID).newHLMoveJob(0,place,0)
		end
	end

	def attack(heroId,targetId)
		if heroOk(heroID) and (not buildingOk(targetId))
			getHero(heroID).newHLFightJob(getHero(targetId))
		end
	end
	
	def recruit(heroId,targetId)
		if heroOk(heroID) and buildingOk(targetId)
			getHero(heroID).newHLRecruitJob(getHero(targetId))
		end
	end

	def takeFood(heroId,targetId)
		puts "TAKING food...."
buildingOk(targetId)	
	puts "---"

		if heroOk(heroId) and buildingOk(targetId)
			puts "yay"
			getHero(heroId).newHLTakeFoodJob(getHero(targetId))
		end
		puts "boooooooooooaa"
		puts "#{heroOk(heroId)} and #{buildingOk(targetId)}"
	end

	def takeWeapon(heroId,targetId)
		if heroOk(heroID) and buildingOk(targetId)
			getHero(heroID).newHLTakeWeaponJob(getHero(targetId))
		end
	end

	def attackAnimal(heroId,targetId)
		if heroOk(heroID)
			getHero(heroID).newHLFightAnimalJob(getHero(targetId))
		end
	end

	def build(heroId,place,type)
		if heroOk(heroID)
			getHero(heroID).newHLBuildJob(place,type)
		end
	end

	def rest(heroId,time)
		if heroOk(heroId)
			getHero(heroId).newHLRestJob(time)
		end
	end

	private
	def heroOk(heroId)
		@map.getHeroes.select{|h|h.getPlayer and h.getPlayer.getName==@player and h.uid==heroId}.length!=0
	end
	def buildingOk(houseId)
		@map.getBuildings.select{|h|h.getPlayer and h.getPlayer.getName==@player and h.uid==houseId}.length!=0
	end
	def getHero(heroId)
		@map.getByUID(heroId)
	end
end

class AIInterface
	include AIActionInterface
	include AISensorInterface

	def initialize(map,player)
		@map=map
		@player=player
	end

end


class AIInterfaceTools<AIInterface
	def findNearestEnemy(heroId)
	end


	def getWeakestEnemyHouse(hero)
		bs=[]
		getPlayerNames.select{|n|n!=@player}.each{|p|
			bs+=getBuidlings(p)
		}
		hs=bs.sort{|a,b|a.getMen.length<=>b.getMen.length}
		hs[0]
	end

end
