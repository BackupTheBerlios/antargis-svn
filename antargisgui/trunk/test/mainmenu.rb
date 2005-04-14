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
require 'antApp.rb'

include Libantargisruby

#
# TODO: change to menu selection, so items get shown and hidden
#

class AntMenuApp <AntApp
	def initialize(autoexit=true)
		super()
		
		# init screen
		$screen=AGScreenWidget.new
		$screen.setName("SCREEN")
		setMainWidget($screen)
		
		setupMain
	end
	
	def clearScreen
		puts "CLEARSCREEN"
		$screen.clear
		clearHandlers
		puts "CLEARSCREEN."
	end
	
	def setupMain()
		puts "SETUPMAIN"
		$screen.clear
		$screen.addChild(AGLayout.new($screen,loadFile("mainmenu.xml")))
		addHandler($screen.getChild("quit"),:sigClick,:sigQuit)
		addHandler($screen.getChild("credits"),:sigClick,:setupCredits)
		puts "SETUPMAIN."
	end
	
	def setupCredits(eventName,callerName,event,caller)
		puts "SETUPCREDITS"
		$screen.clear
		$screen.addChild(AGLayout.new($screen,loadFile("credits.xml")))
		addHandler($screen.getChild("exit"),:sigClick,:sigExit)
	end
	
	def sigExit(eventName,callerName,event,caller)
		setupMain
	end
	
	def sigQuit(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		tryQuit
	end
	
end


main=AGMain.new

main.changeRes(1024,768,32,false,true)

app=AntMenuApp.new(false)

app.run

