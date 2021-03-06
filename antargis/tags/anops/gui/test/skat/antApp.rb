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

#require 'libantargis'
#include Libantargis
#
# EVENT_HANDLERS
#
# this module adds simpler event handling to your ruby code
# each slot should have for parameters: (eventname,callerName,eventItself,callerHimself)
# add a slot by addHandler(object,:signalName,:slotName)
# and also include this module into each class in which you want to use this :-)
module AGHandler
	def clearHandlers
		@handlers={}
	end
	# add Event Handler - this function should go into AGRubyApp
	def addHandler(object,event,func)
		if not defined? @handlers then
			@handlers={}
		end
		puts event
		if object==nil then
			puts "AGHandler.addHandler: does not exist!"
		else
			object.send(event).connect(self)
			@handlers[object.getName+":"+event.to_s]=func
		end
	end
	# event dispatcher
	def signal(name,event,caller)
		if not defined? @handlers then
			@handlers={}
		end
		callerName=toAGWidget(caller).getName
		evName=callerName+":"+name
		puts self
		puts self.methods
		puts "varS:"
		puts instance_variables
		puts @handlers
		if @handlers.has_key?(evName) then
			m=method(@handlers[evName])
			if m.arity==4 then
				if self.send(@handlers[evName],name,callerName,event,caller) then
					return true
				end
			else
				if self.send(@handlers[evName]) then
					return true
				end
			end
		end
		return super(name,event,caller)
	end
end

class AntApp <AGApplication
	include AGHandler
	def initialize(autoexit=true)
		super()
	end
	def eventQuit(event)
		puts "Quitting AntApp"
		super(event)
	end
end

