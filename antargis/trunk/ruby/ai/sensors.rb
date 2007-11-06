module AI
	class Sensor
		attr_reader :player

		def initialize(player)
			@player=player
		end

		def logValue
			Math.log(value)
		end
	end

	class HeroFoodSensor<Sensor
		def value
			heroFood=player.getHeroes.map{|h|h.resources.get("food")}

			heroFood.inject(0){|a,b|a+b}
		end
	end

	class StrengthSensor<Sensor
		def value
			heroStrengths=player.getHeroes.map{|h|h.menCount}
			heroStrengths.inject(0){|a,b|a+b}
		end
	end

	class BuildingCountSensor<Sensor
		def value
			player.getBuildings.length
		end
	end

	class CompletenessOfProdChainSensor<Sensor
		MAX_DIST=20

		def value
			blocks=createBlocks(player.getBuildings.map{|b|[b]})
			blocks.map{|block|
				completenessOfChain(block)
			}

		end
	private
		def completenessOfChain(block)
			
		end
		def createBlocks(blocks)
			nblocks=blocks.inject([]){|blocklist,newBuilding|
				added=false
				blocklist.each{|group|
					group.each{|currentBuilding|
						if (currentBuilding.getPos2D-newBuilding.getPos2D).length<MAX_DIST
							blocklist=blocklist-[group]+[group+newBuilding]
							added=true
							break
						end
					}
				}
				blocklist+=[newBuilding] unless added
			}
			nblocks=createBlocks(nblocks) if nblocks.length!=blocks.length
			nblocks
		end

	end

	SENSORS=[
		HeroFoodSensor,
		StrengthSensor,
		BuildingCountSensor,
		CompletenessOfProdChainSensor
	]	

	

end