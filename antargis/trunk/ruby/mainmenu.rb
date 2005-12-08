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

require 'ruby/antargislib.rb'
require "antargis.rb"

class AntMenuApp <AGApplication
	include AGHandler
	def initialize
		super()
		
		# init menues
		@menues=[]	
		setupMain
		setupCredits
		setupCampaign
		setupOptions
		# hie all menues
		#hideAll
		# and show mainmenu
		@mainMenu.show
		@sound=true
		addHandler(getSoundManager,:sigMp3Finished,:musicEnd)
	end
	
	def musicEnd
		if @sound
			getSoundManager.playMp3("data/music/ant2.ogg")
		end
	end
		
	def hideAll
		@menues.each {|x|x.hide}
	end
	
	def setupMain()
		puts "SETUPMAIN"
		@mainMenu=AGLayout.new($screen,loadFile("data/gui/layout/mainmenu.xml"))
		@menues.push(@mainMenu)
		setMainWidget(@mainMenu)
		#$screen.addChild(@mainMenu)
		@mainMenu.hide
		addHandler(@mainMenu.getChild("quit"),:sigClick,:sigQuit)
		addHandler(@mainMenu.getChild("credits"),:sigClick,:sigCredits)
		addHandler(@mainMenu.getChild("campaign"),:sigClick,:sigCampaign)
		addHandler(@mainMenu.getChild("options"),:sigClick,:sigOptions)
		
		puts "SETUPMAIN."
	end
	
	def setupCredits
		puts "SETUPCREDITS"
		@creditsMenu=AGLayout.new($screen,loadFile("data/gui/layout/credits.xml"))
		@menues.push(@creditsMenu)
		addHandler(@creditsMenu.getChild("exit"),:sigClick,:sigExit)
	end
	
	def setupCampaign
		puts "SETUPCAMPAIGN"
		@campaignMenu=AGLayout.new($screen,loadFile("data/gui/layout/campaign.xml"))
		@menues.push(@campaignMenu)
		addHandler(@campaignMenu.getChild("exit"),:sigClick,:sigExit)
		addHandler(@campaignMenu.getChild("startWolf"),:sigClick,:sigMission)
	end
	
	def setupOptions
		puts "SETUPOPTIONS"
		@optionsMenu=AGLayout.new($screen,loadFile("data/gui/layout/options.xml"))
		@menues.push(@optionsMenu)
		addHandler(@optionsMenu.getChild("exit"),:sigClick,:sigExit)
	end
	
	# Mainmenu-sigs
	
	def sigCredits(eventName,callerName,event,caller)
		setMainWidget(@creditsMenu)
	end
	def sigCampaign(eventName,callerName,event,caller)
		setMainWidget(@campaignMenu)
	end
	def sigOptions(eventName,callerName,event,caller)
		setMainWidget(@optionsMenu)
	end
	def sigQuit(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		tryQuit
	end
	
	def sigMission(eventName,callerName,event,caller)
		puts "caller:"+callerName
		case callerName
			when "startWolf"
				soundOff
				startGame
				soundOn
		end
	end

	# all exits to mainmenu	
	def sigExit(eventName,callerName,event,caller)
		setMainWidget(@mainMenu)
	end
	def eventIdle
		delay(20)
	end
	def soundOff
		@sound=false
		getSoundManager.fadeOutMusic(1000)
	end
	def soundOn
		@sound=true
		musicEnd
	end	
	
end


if not $main
	$main=AGMain.new(1024,768,32,false,true)
end

app=AntMenuApp.new

app.run

