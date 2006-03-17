#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hljobs.rb
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

#!/usr/bin/ruby


module AntManBase
	def sitDown
		newRestJob(0.4)
		setMeshState("sitdown")
	end

	def setMode(mode)
		@mode=mode
	end
	def getMode
		@mode
	end
	def eventGotFight(other)
		return
		puts "EVENTGOTFIGHT"
		if shouldFight and (not isFighting)
			#newFightJob(0,other)
		end
	end
	def eventDefeated
		if is_a?(AntBoss)
			eventManDefeated(self)
		elsif @boss then
			@boss.eventManDefeated(self)
			#@boss.removeMan(self)
			#@boss=nil
			#@bossName=""
		end
	end


	def shouldFight
		canFight
	end
end