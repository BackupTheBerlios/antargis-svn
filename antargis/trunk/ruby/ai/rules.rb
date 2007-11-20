require 'ruby/ai/hero_sensors.rb'
require 'ruby/ai/hero_actions.rb'

module AI
	module Rule
		class Base
			attr_reader :interface
			def initialize(interface)
				@interface=interface
			end
			def start(hero)
			end
			def value(hero)
			end
		end

		class GetWeapon<Base
			def initialize(interface)
				super
				@eqNear=HeroSensor::EquipmentNear.new(interface)
				@getEq=HeroAction::GetEquipment.new(interface)
			end
			def value(hero)
				eqNear=@eqNear.value(hero)
			end
			def start(hero)
				puts "START WEAPONS"
				@getEq.start(hero,@eqNear.target)
			end
		end

	
		class GetFood<Base
			def initialize(interface)
				super
				@foodLow=HeroSensor::FoodLow.new(interface)
				@foodNear=HeroSensor::FoodNear.new(interface)
				@getFood=HeroAction::GetFood.new(interface)
			end
			def value(hero)
				foodLow=@foodLow.value(hero)
				foodNear=@foodNear.value(hero)
				puts "FOODLOW:#{foodLow} FOODNEAR:#{foodNear}"
				foodLow*foodNear
			end
			def start(hero)
				puts "START GETFOOD"
				@getFood.start(hero,@foodNear.target) #.getSource(hero))
			end
		end
	
		class AttackHero<Base
			def initialize(interface)
				super
				@strength=HeroSensor::Strength.new(interface)
				@weakEnemy=HeroSensor::WeakEnemy.new(interface)
				@attackHero=HeroAction::AttackHero.new(interface)
			end
			def value(hero)
				# FIXME: include sensor for multiple heroes at same place
				@weakEnemy.value(hero)*@strength.value(hero)
			end
			def start(hero)
				puts "START ATTACK"
				@attackHero.start(hero,@weakEnemy.enemy)
			end
		end

		class RecruitFromBuilding<Base
			def initialize(interface)
				@weakEnemy=HeroSensor::WeakEnemy.new(interface)
				@strongBuilding=HeroSensor::StrongBuilding.new(interface)
				@recruit=HeroAction::Recruit.new(interface)
			end
			def value(hero)
				# FIXME: include sensor for multiple heroes at same place
				# FIXME: switch to strong_building AND not_insecure (attackable)
				#Math.sqrt((1-@weakEnemy.value(hero))*@strongBuilding.value(hero))
				@strongBuilding.value(hero)
			end
			def start(hero)
				puts "TRY RECRUIT"
				@recruit.start(hero,@strongBuilding.target)
			end
		end

		class Rest<Base
			def initialize(interface)
				super
				@foodok=HeroSensor::FoodOk.new(interface)
				@tired=HeroSensor::Tired.new(interface)
				@hungry=HeroSensor::Hungry.new(interface)
				@rest=HeroAction::Rest.new(interface)
			end
			def value(hero)
				[@hungry.value(hero),@tired.value(hero),0.2].max
				#0.2
				#@foodok.value(hero)
			end
			def start(hero)
				@rest.start(hero)
			end
		end

		class Construct<Base
			def initialize(interface)
				super
				@badEquipment=HeroSensor::BadEquipment.new(interface)
				@workshopNear=HeroSensor::WorkshopNear.new(interface)
				@construct=HeroAction::ConstructEquipment.new(interface)
			end
			def value(hero)
				Math.sqrt(@badEquipment.value(hero)*@workshopNear.value(hero))
			end
			def start(hero)
				@construct.start(hero,@workshopNear.target)
			end
		end

	end
end