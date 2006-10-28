#--
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
#++
# == BoA AI
# The AI within BoA is based upon the AntBasicAI class. So every AI you write has to be derived from this.
# Within such an implementation you're allowed to use an interface, which is defined in AIInterface. For
# more information on this refer to link:files/ruby/ant_ai_interface_rb.html.
#
# For more detailed information on how to program an AI go to  link:files/ruby/ant_ai_interface_rb.html or to 
# link:../tutorials/ai_scripting.html

require 'ant_ai_interface.rb'
require 'ant_ai_rules.rb'
require 'ant_ai_player.rb'

class AntBasicAI
	attr_reader :interface

	# initialize any information for your AI here, but beware that this is called on reload, too.
	# FIXME: loading and saving of AI-members!
	def initialize(interface)
		@interface=interface
	end
	# This an initializing event - this called only on new creation within a level - so it won't be
	# called when loading a level. You can savely initialize your values here.
	def eventInit
	end
	# the given hero has no job currently 
	def eventHeroWithoutJob(hero)
	end
	# the given hero is attacked by the given enemy
	def eventHeroAttacked(hero,enemy)
	end
	# your hero has hit a trigger - this SHOULDN'T really BE USED, because triggers are only
	# reasonable within level-scripting (?) 
	def eventTriggerHit(hero,triggerName)
	end
	# in each frame this function is called with the time that's passed. Don't use this function if you
	# don't need to, because this hits performance!
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

