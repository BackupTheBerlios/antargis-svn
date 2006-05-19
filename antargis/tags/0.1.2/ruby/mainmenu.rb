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
$useMenu=true

require "antargis.rb"
require 'campaign.rb'
require 'credits.rb'

class AntMenuApp <AGApplication
	include AGHandler
	def initialize
		super()
		$menuapp=self
		# init menues
		@menues=[]	
		setupMain
		setupCredits
		setupCampaign
		setupOptions
		setupLoadMenu
		setupSingle
		# hie all menues
		#hideAll
		# and show mainmenu
		@mainMenu.show
		@sound=true
		addHandler(getSoundManager,:sigMp3Finished,:eventMusicEnd)
	end
	
	def eventMusicEnd
		if @sound
			getSoundManager.playMp3("data/music/ant2.ogg")
		end
		return true
	end
		
	def hideAll
		@menues.each {|x|x.hide}
	end
	
	def setupMain()
		@mainMenu=AGLayout.new($screen)
		@mainMenu.loadXML(loadFile("data/gui/layout/mainmenu.xml"))
		@menues.push(@mainMenu)
		setMainWidget(@mainMenu)
		#$screen.addChild(@mainMenu)
		@mainMenu.hide
		addHandler(@mainMenu.getChild("quit"),:sigClick,:eventQuit)
		addHandler(@mainMenu.getChild("credits"),:sigClick,:eventCredits)
		addHandler(@mainMenu.getChild("campaign"),:sigClick,:eventCampaign)
		addHandler(@mainMenu.getChild("single"),:sigClick,:eventSingle)
		addHandler(@mainMenu.getChild("load"),:sigClick,:eventLoadGame)
		addHandler(@mainMenu.getChild("options"),:sigClick,:eventOptions)
	end

	def setupLoadMenu
		@loadMenu=AGLayout.new($screen)
		@loadMenu.loadXML(loadFile("data/gui/layout/loadmenu.xml"))
		@menues.push(@loadMenu)
		addHandler(@loadMenu.getChild("exit"),:sigClick,:eventExit)
		addHandler(@loadMenu.getChild("list"),:sigSelect,:eventLoadSelect)
		addHandler(@loadMenu.getChild("doLoad"),:sigClick,:eventLoad)
		updateLoadMenu
	end

	def updateLoadMenu
		fs=getDirectory(getWriteDir+"/savegames").select{|f|f=~/\.antcmp/}.sort.uniq
		puts fs.join(" ")
		l=@loadMenu.getChild("list")
		l.clearList
		fs.each{|f|
			x=f.gsub(".antcmp","")
			l.insertItem(f,x)
		}
		#exit
	end
	
	def setupCredits
		@creditsMenu=AGLayout.new($screen)
		@creditsMenu.loadXML(loadFile("data/gui/layout/credits.xml"))
		@menues.push(@creditsMenu)
		addHandler(@creditsMenu.getChild("exit"),:sigClick,:eventExit)
	end

	def setupSingle
		@singleMenu=AGLayout.new($screen)
		@singleMenu.loadXML(loadFile("data/gui/layout/single.xml"))
		@menues.push(@singleMenu)
		addHandler(@singleMenu.getChild("singleExit"),:sigClick,:eventExit)
		addHandler(@singleMenu.getChild("singleStart"),:sigClick,:eventStart)
	end
	
	def setupCampaign
		@campaignMenu=AGLayout.new($screen)
		@campaignMenu.loadXML(loadFile("data/gui/layout/campaign.xml"))
		@menues.push(@campaignMenu)
		addHandler(@campaignMenu.getChild("exit"),:sigClick,:eventExit)
		addHandler(@campaignMenu.getChild("start"),:sigClick,:eventStart)
		
		@campaigns=getCampaigns
		
		buttonCount=6
		campaignButtons=(0..(buttonCount-1)).to_a.collect{|c|"campaign#{c}"}
		i=0
		campaignButtons.each{|b|
			c=@campaignMenu.getChild(b)
			addHandler(c,:sigClick,:eventMission)
			if @campaigns.length>i
				c.setCaption(_(@campaigns[i].name))
			else
				c.hide
			end
			i+=1
		}
	end
	
	def setupOptions
		@optionsMenu=AGLayout.new($screen)
		@optionsMenu.loadXML(loadFile("data/gui/layout/options.xml"))
		@menues.push(@optionsMenu)
		addHandler(@optionsMenu.getChild("exit"),:sigClick,:eventExit)
		
		addHandler(@optionsMenu.getChild("fullscreen"),:sigClick,:eventFullscreen)
		addHandler(@optionsMenu.getChild("1024"),:sigClick,:eventResChange)
		addHandler(@optionsMenu.getChild("1280"),:sigClick,:eventResChange)
		addHandler(@optionsMenu.getChild("1400"),:sigClick,:eventResChange)
	end
	def eventFullscreen
		getMain.changeRes(getMain.width,getMain.height,32,(not getMain.fullscreen),true)
		return true
	end

	def eventResChange(e)
		case e.getCaller.getName
			when "1024"
				setRes(1024,768)
			when "1280"
				setRes(1280,1024)
			when "1400"
				setRes(1400,1050)
		end
		return true
	end
	def setRes(w,h)
		getMain.changeRes(w,h,32,true,true) #getMain.fullscreen,true)
		puts getSurfaceManager.getUsedTexMem
		#raise 1
	end
	# Mainmenu-sigs
	
	def eventCredits(e)
		setMainWidget(@creditsMenu)
		return true
	end
	def eventSingle(e)
		setMainWidget(@singleMenu)
		return true
	end

	def eventCampaign(e)
		setMainWidget(@campaignMenu)
		return true
	end
	def eventLoadGame(e)
		updateLoadMenu
		setMainWidget(@loadMenu)
		return true
	end
	def eventOptions(e)
		setMainWidget(@optionsMenu)
		return true
	end
	def eventQuit(e)
		tryQuit
		return true
	end
	
	def eventMission(e)
		callerName=e.getCaller.getName
		number=callerName[8..12].to_i
		@selCampaign=@campaigns[number]
		@campaignMenu.getChild("campaignImage").setTexture(@selCampaign.texture)
		@campaignMenu.getChild("campaignDescription").setText(_(@selCampaign.description))
		return true
	end
	
	def eventStart(e)
		puts "EVENTSTART"
		if @selCampaign
			soundOff
			#startGame(@selCampaign)
			startCampaign(@selCampaign)
			soundOn
		end
		return true
	end

	# all exits to mainmenu	
	def eventExit(e)
		setMainWidget(@mainMenu)
		return true
	end
	def eventIdle
		delay(20)
		return true
	end

	def eventFrame(t)
		@frameTime||=0
		@frames||=0
		@frameTime+=t
		@frames+=1
		if @frames>10
			puts "FPS:#{10.0/@frameTime}"
			@frameTime=0
			@frames=0
		end
		return true
	end

	def soundOff
		@sound=false
		getSoundManager.fadeOutMusic(1000)
	end
	def soundOn
		@sound=true
		eventMusicEnd
	end	

	# load menu
	def eventLoadSelect(e)
		puts "LOAD SELECT"
		@loadMenu.getChild("desc").setText("muh")
		return true
	end
	def eventLoad(e)
		id=@loadMenu.getChild("list").getSelectedID
		puts "LLLLLLLLLLLLLLOAD"
		puts id
		if id!=""
			c=Campaign.new(getWriteDir+"/savegames/"+id)
			continueCampaign(c)
		end
		setMainWidget(@mainMenu)
		return true
	end
end


if not $main
	$main=AGMain.new(1024,768,32,false,true)
end

app=AntMenuApp.new

app.run

