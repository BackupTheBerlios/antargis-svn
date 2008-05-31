require 'value_map.rb'

module AI
  class Sensor
    attr_reader :player

    def initialize(player)
      @player=player
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

  class InDangerSensor<Sensor
  end

  # the opposite of InDangerSensor
  class GoodAttackPosSensor<Sensor
  end

  # enough resources in vicinity of buildings
  class GoodResourceSensor<Sensor
    RESOURCES=[:wood,:stone,:ore]
    MAX_DIST=50
    MIN_RESOURCE=5
    def value
      
      player.getBuildings.map{|building|
        RESOURCES.map{|resource|
          map.getNext(building,resource,MIN_RESOURCE).map{|r|
            mapValue01(MIN_RESOURCE,r.resource.get(resource))
          }.min
        }
      }
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
      #FIXME
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