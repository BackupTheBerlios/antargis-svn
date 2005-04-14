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
require 'sdl'
#require 'testapp.rb'
#require 'theme.rb'

include Libantargisruby


class MenuApp <AGApplication
	def initialize(autoexit=true)
		@count=0
		@autoexit=autoexit
		@handlers={}
		super()
		
		# init screen
		$screen=AGScreenWidget.new
		$screen.setName("SCREEN")
		
		# load layout
		l=AGLayout.new($screen,loadFile("mainmenu.xml"))
		$screen.addChild(l)
		
		# setup eventhandling
		c=$screen.getChild("quit")
		#c.sigClick.connect(self)
		addHandler(c,:sigClick,:test2) # FIXME: this doesn't work yet
		setMainWidget($screen)
		#ObjectSpace.garbage_collect
	end
	def eventQuit(event)
		puts "Quitting"
		super(event)
	end
	def eventIdle
		#puts "idle count:"+@count.to_s
		@count+=1
		if @count>=20 then
			if @autoexit then
				puts "correct quit after 20 idles"
				tryQuit
			end
		end
	end
	
	
	def test2(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		tryQuit
	end
	
	
	# add Event Handler - this function should go into AGRubyApp
	def addHandler(object,event,func)
		puts event
		object.method(event).call.connect(self)
		@handlers[object.getName+":"+event.to_s]=func
	end
	# event dispatcher
	def signal(name,event,caller)
		callerName=toAGWidget(caller).getName
		#puts "signal:"+name
		#puts "pCaller:"+toAGWidget(caller).getName
		#if callerName="quit" then tryQuit end
		evName=callerName+":"+name
		if @handlers.has_key?(evName) then
			self.method(@handlers[evName]).call(name,callerName,event,caller)
			return true
		end
		
		return super(name,event,caller)
	end
	def eventQuit2(name,event)
	end
end


#puts "MenuTest"

main=AGMain.new

#doc=Document.new("theme.xml")
#puts doc
#loadTheme(loadFile("thedoc.toString)

main.changeRes(1024,768,32,false,true)

app=MenuApp.new(false)

app.run

