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
require 'antApp.rb'

include Libantargisruby

class AntMenuApp <AntApp
	def initialize()
		super()
		
		# init screen
		$screen=AGScreenWidget.new
		$screen.setName("SCREEN")
		setMainWidget($screen)
	
		# init menues
		@menues=[]	
		setupMain
		setupCredits
		# hie all menues
		hideAll
		# and show mainmenu
		@mainMenu.show
	end
	
	def hideAll
		@menues.each {|x|x.hide}
	end
	
	def setupMain()
		puts "SETUPMAIN"
		@mainMenu=AGLayout.new($screen,loadFile("mainmenu.xml"))
		@menues.push(@mainMenu)
		$screen.addChild(@mainMenu)
		@mainMenu.hide
		addHandler($screen.getChild("quit"),:sigClick,:sigQuit)
		addHandler($screen.getChild("credits"),:sigClick,:sigCredits)
		puts "SETUPMAIN."
	end
	
	def setupCredits
		puts "SETUPCREDITS"
		@creditsMenu=AGLayout.new($screen,loadFile("credits.xml"))
		@menues.push(@creditsMenu)
		$screen.addChild(@creditsMenu)
		addHandler($screen.getChild("exit"),:sigClick,:sigExit)
	end
	
	def sigCredits(eventName,callerName,event,caller)
		hideAll
		@creditsMenu.show
	end
	
	def sigExit(eventName,callerName,event,caller)
		hideAll
		@mainMenu.show
	end
	
	def sigQuit(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		tryQuit
	end
	
end


main=AGMain.new

main.changeRes(1024,768,32,false,true)

app=AntMenuApp.new()

app.run

