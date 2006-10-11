#
# Copyright (c) 2006 by David Kamphausen. All rights reserved.
#
# ant_ai_rules.rb
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
# Simple rules-based system:
# A rule consists of a condition and one or more actions
# These rules are processed in pre-defined order - look at ant_ai.rb for more
#


class AICondition
	def value(interface,hero)
		return 1
	end
end

class AIConditionFoodLow<AICondition
	def value(interface,hero)
		if interface.getResourceInfo(hero,"food")<5 and interface.isAppliableResourceNear(hero,"food")
			return 1
		else
			return 0
		end
	end
end

class AIEveryNthCondition<AICondition
	def initialize(nth)
		@nth=nth
		@v=0
	end
	def value(interface,hero)
		@v+=1
		@v%=@nth
		return @v==0?1:0
	end
end


# Dummy Action - does nothing at all
class AIAction
	def execute(interface,hero)
	end
end

# get food from some building which is owned by the hero
class AIGatherFoodAction<AIAction
	def execute(interface,hero)
		unit=interface.findResource(hero,"food")
		puts "HERO TAKES FOOD"
		interface.takeFood(hero,unit)
	end
end

# rest for a second
class AIRestAction<AIAction
	def execute(interface,hero)
		interface.rest(hero,1)
	end
end

# build up a rule out of a condition and one or more actions:
# AIRule.new(AICondition.new,AIRestAction.new)
# or
# AIRule.new(AICondition.new,[AIRestAction.new,Someotheraction.new])
class AIRule
	def initialize(condition,actions)
		@c=condition
		@a=actions
	end
	def value(interface,hero)
		@c.value(interface,hero)
	end
	def execute(interface,hero)
		if @a.is_a?(Array)
			@a.each{|a|a.execute(interface,hero)}
		else
			@a.execute(interface,hero)
		end
	end
end