
# DELETE THIS ONE!


module AI
	class Heuristic
		def initialize
			classes=[
				[AI::Rule::AttackHero,1],
				[AI::Rule::GetFood,1]
			]
			@rules=classes.map{|pair|[pair[0].new,scale]}
		end


		def process(hero)
			bestRule=@rules.max{|pair|
				rule,scale=pair
				rule.value(hero)*scale
			}
			bestRule.start(hero)
		end
	end
end