require 'ruby/ai/value_map.rb'

module AI
  module HeroSensor
    NEEDED_EQUIPMENT={"sword"=>2,
      "shield"=>2,
      "bow"=>1,
      "boat"=>1
    }

    class Base
      attr_reader :interface
      def initialize(interface)
        @interface=interface
      end

      def value(hero)
        0
      end
    end

    class FoodOk<Base
      def value(hero)
        puts "FOODLOW:#{hero.resources["food"]}"
        mapValue01(hero.resources["food"]||0,1,5)
      end
    end

    class FoodLow<Base
      def value(hero)
        puts "FOODLOW:#{hero.resources["food"]}"
        1-mapValue01(hero.resources["food"]||0,1,5)
      end
    end

    class FoodNear<Base
      # BEWARE: Do not use this variable after several calls of value!
      attr_reader :target
      def value(hero)
        @target=hero.findNearResource("food")
        return 0 unless @target
        distance=(@target.getPos-hero.getPos).length
        puts "FOODNEAR: dist:#{distance}"
        mapValue01(40-distance,0,40)
      end
    end

    # FIXME: is building reachable ??
    class EquipmentNear<Base
      attr_reader :target
      def value(hero)
        assert{hero.menCount>0}
        puts "--EquipmentNear--"
        needed=NEEDED_EQUIPMENT.map{|res|[res[0],[0,hero.menCount-(hero.resources[res[0]]||0)].max]}
        howMuchIsMissing=needed.inject(0){|result,nextPair|result+nextPair[1]}

        houses=interface.myPlayer.getBuildings
        return if houses.length==0
        houseValues=houses.map{|house|[house,getResourceValue(needed,house)]}
        selHouse=houseValues.min{|a,b|a[1]<=>b[1]}

        puts selHouse,howMuchIsMissing
        puts "--"
        return 0 if selHouse[1]>=howMuchIsMissing

        @target=selHouse[0]
        

        puts needed
        puts howMuchIsMissing
        puts hero.menCount
        value=howMuchIsMissing.to_f / (hero.menCount*NEEDED_EQUIPMENT.length)
        puts value
        puts "--EquipmentNear--"
        value
      end

      # return what's missing from that house
      def getResourceValue(needed,house)
        puts "HOUSE:#{house.getName} #{house}"
        result=needed.inject(0){|result,nextPair|
          res,value=nextPair
          found=house.resources[res]||0
          puts "#{res} #{res.class} #{value} #{found}"
          result+=[value-found,0].max
        }
          #raise 1
        result
      end
    end


    class Strength<Base
      def value(hero)
        strength=hero.menCount
        puts "STRENGTH #{strength}"
        val=mapValueLinear(strength,0,40) # not 0, so value is never 0
        puts "val #{val}"
        val
      end
    end

    class WeakEnemy<Strength # !!
      attr_reader :enemy
      def value(hero)
        # search weakest enemy - FIXME: ensure that it's reachable
        @enemy=interface.enemyPlayers.map{|p|p.getHeroes+p.getBuildings}.flatten.min{|a,b|a.menCount<=>b.menCount}
  
        return 0 if @enemy.nil?
        quot=super(hero) / (super(@enemy)*1.2) # expect enemy to be stronger by 20%
        log=Math.log(quot)
        val=mapValue01(log,-1,2)
        puts "weak: quot:#{quot} log:#{log} val:#{val}"
        val
      end
    end

    class StrongBuilding<Strength
      attr_reader :target
      def value(hero)
        # search strongest building - FIXME: ensure that it's reachable
        targetArray=[interface.myPlayer].map{|p|p.getHeroes+p.getBuildings}.flatten.select{|h|h!=hero}
        @target=targetArray.max{|a,b|a.menCount<=>b.menCount}
        return 0 if @target.nil?
        strength=@target.menCount/(hero.menCount+0.001)
        return mapValueLinear(strength,0,3)
      end
    end

    class Tired<Base
      def value(hero)
        1-[hero.getMorale,hero.getEnergy**2].min
      end
    end
    class Hungry<Base
      def value(hero)
        1-[hero.getFood**2].min
      end
    end

    class BadEquipment<Base
      SCALE=NEEDED_EQUIPMENT.values.inject(0){|a,b|a+b}.to_f
      def value(hero)
        hero.resources.map{|r,v|(NEEDED_EQUIPMENT[r]||0)*(v>0?1:0)}.inject(0){|a,b|a+b}/SCALE
      end
    end

    class WorkshopNear<Base
      attr_reader :target
      def value(hero)
        targetArray=interface.myPlayer.getBuildings.select{|b|b.isBuildingType("AntWorkshop")}
        @target=targetArray.max{|a,b|(a.getPos2D-hero.getPos2D).length<=>(b.getPos2D-hero.getPos2D).length}
        return 0 unless @target
        1-mapValueLinear((@target.getPos2D-hero.getPos2D).length,0,30)
      end
    end

  end
end