#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# testapp.rb
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

require 'antargisgui.rb'

class TestApp <AGApplication
	def initialize(autoexit=true)
#		initTheme
		@count=0
		@autoexit=autoexit
		@frames=0
		@mtime=0
		super()
	end
	def eventQuit(event)
		puts "Quitting"
		super(event)
	end
	def eventFrame(t)
		@frames+=1
		@mtime+=t
		if @frames==10
			puts 10/@mtime
			@frames=0
			@mtime=0
		end
		delay(10)
		return false
	end
	def eventIdle
		#delay(10)
		#puts "idle count:"+@count.to_s
		@count+=1
		if @count>=20 then
			if @autoexit then
				puts "correct quit after 20 idles"
				tryQuit
			end
		end
		delay(100)
		return false
	end
# 	def getNewEvent
# 		super
# 	end
end
