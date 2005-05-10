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
include Libantargisruby
require 'antApp.rb'


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
		setupCampaign
		setupOptions
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
		addHandler(@mainMenu.getChild("quit"),:sigClick,:sigQuit)
		addHandler(@mainMenu.getChild("credits"),:sigClick,:sigCredits)
		addHandler(@mainMenu.getChild("campaign"),:sigClick,:sigCampaign)
		addHandler(@mainMenu.getChild("options"),:sigClick,:sigOptions)
		puts "SETUPMAIN."
	end
	
	def setupCredits
		puts "SETUPCREDITS"
		@creditsMenu=AGLayout.new($screen,loadFile("credits.xml"))
		@menues.push(@creditsMenu)
		$screen.addChild(@creditsMenu)
		addHandler(@creditsMenu.getChild("exit"),:sigClick,:sigExit)
	end
	
	def setupCampaign
		puts "SETUPCAMPAIGN"
		@campaignMenu=AGLayout.new($screen,loadFile("campaign.xml"))
		@menues.push(@campaignMenu)
		$screen.addChild(@campaignMenu)
		addHandler(@campaignMenu.getChild("exit"),:sigClick,:sigExit)
	end
	
	def setupOptions
		puts "SETUPOPTIONS"
		@optionsMenu=AGLayout.new($screen,loadFile("options.xml"))
		@menues.push(@optionsMenu)
		$screen.addChild(@optionsMenu)
		addHandler(@optionsMenu.getChild("exit"),:sigClick,:sigExit)
	end
	
	# Mainmenu-sigs
	
	def sigCredits(eventName,callerName,event,caller)
		hideAll
		@creditsMenu.show
	end
	def sigCampaign(eventName,callerName,event,caller)
		hideAll
		@campaignMenu.show
	end
	def sigOptions(eventName,callerName,event,caller)
		hideAll
		@optionsMenu.show
	end
	def sigQuit(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		tryQuit
	end

	# all exits to mainmenu	
	def sigExit(eventName,callerName,event,caller)
		hideAll
		@mainMenu.show
	end
	
	
end


main=AGMain.new

main.changeRes(1024,768,32,false,true)

app=AntMenuApp.new()

app.run

