#
# Copyright (c) 2006 by David Kamphausen. All rights reserved.
#
# ant_ai.rb
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

require 'ant_ai_interface.rb'
require 'ant_ai_rules.rb'
require 'ant_ai_player.rb'

class AntBasicAI
	attr_reader :interface

	def initialize(interface)
		@interface=interface
	end
	def eventInit
	end
	def eventHeroWithoutJob(hero)
	end
	def eventHeroAttacked(hero,enemy)
	end
	def eventTriggerHit(heroId,triggerName)
	end
	def eventFrame(time)
	end
end

class AntAttackAI<AntBasicAI
	def initialize(interface)
		super
		# add rules
		@rules=[]
		@rules.push(AIRule.new(AIEveryNthCondition.new(2),AIRestAction.new,interface))
		@rules.push(AIRule.new(AIConditionFoodLow.new,AIGatherFoodAction.new,interface))
		@rules.push(AIRule.new(AIEveryNthCondition.new(2),AIAttackAction.new,interface))
		@rules.push(AIRule.new(AICondition.new,AIRestAction.new,interface)) # dummy rule - always rest
	end

	def eventHeroWithoutJob(hero)
		@rules.each{|r|
			if r.value(hero)>0
				puts "GO"
				r.execute(hero)
				return
			end
		}
	end
end

