module AI
	class Action
		
	end

	class GetFoodAction
		def start
			@hero.newHLTakeFoodJob(@target)
		end
		def estimate(time)
			@time+=time
			
		end

		def self.generate(player)
			actions=player.getHeroes.map{|hero|
				action=nil
				target=hero.findNearResource("food") # FIXME: list of possible resources ???
				if target
					action=self.new(hero,target)
				end
				action
			}
			actions.flatten.uniq-[nil]
		end
		private
		def initialize(hero,target)
			@hero=hero
			@target=target
			@time=0
		end
	end
end