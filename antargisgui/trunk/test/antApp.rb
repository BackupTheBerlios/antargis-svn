#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# layout.rb
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

require 'libantargisruby'
#require 'sdl'
require 'antApp.rb'

include Libantargisruby


class AntApp <AGApplication
	def initialize(autoexit=true)
		@handlers={}
		super()
		
		#example for Handler:
		# c=$screen.getChild("quit")
		# addHandler(c,:sigClick,:test2) 
	end
	def eventQuit(event)
		puts "Quitting AntApp"
		super(event)
	end
	

	def clearHandlers
		@handlers={}
	end
	# add Event Handler - this function should go into AGRubyApp
	def addHandler(object,event,func)
		puts event
		object.send(event).connect(self)
		@handlers[object.getName+":"+event.to_s]=func
	end
	# event dispatcher
	def signal(name,event,caller)
		callerName=toAGWidget(caller).getName
		evName=callerName+":"+name
		if @handlers.has_key?(evName) then
			if self.send(@handlers[evName],name,callerName,event,caller) then
				return true
			end
		end
		return super(name,event,caller)
	end
	def eventQuit2(name,event)
	end
end

