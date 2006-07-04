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

# AntMenuApp implements the whole menu-block of BoA
# It uses the setMainWidget-function of AGApplication
# This way the current menu can easily be exchanged.

class AntMenuApp <AGApplication
	include AGHandler
	def initialize
		super()
		$menuapp=self
		# init menues
		@menues=[]
		
		# setup all the 
		puts "SETUPMAIN"
		setupMain
		puts "SETUPCREDITS"
		setupCredits
		puts "SETUPCAMPAIGN"
		setupCampaign
		puts "SETUPOPTIONS"
		setupOptions
		puts "SETUPLOADMENU"
		setupLoadMenu
		puts "SETUPSINGLE"
		setupSingle
		@sound=true

		# add handler for music finished, simply restart music
		addHandler(getSoundManager,:sigMp3Finished,:eventMusicEnd)
	end
	
	def eventMusicEnd
		if @sound
			getSoundManager.playMp3("data/music/ant2.ogg")
		end
		return true
	end
		
	def setupMain()
		@mainMenu=AGLayout.new($screen)
		@mainMenu.loadXML(loadFile("data/gui/layout/mainmenu.xml"))
		@menues.push(@mainMenu)
		setMainWidget(@mainMenu)
		addHandler(@mainMenu.getChild("quit"),:sigClick,:eventQuit)
		addHandler(@mainMenu.getChild("credits"),:sigClick,:eventCredits)
		addHandler(@mainMenu.getChild("campaign"),:sigClick,:eventCampaign)
		addHandler(@mainMenu.getChild("single"),:sigClick,:eventSingle)
		addHandler(@mainMenu.getChild("tutorial"),:sigClick,:eventTutorial)
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
		if fs.length>0
			fn="savegames/"+fs[0].gsub("antcmp","png")
			if findFile(fn)!=""
				@loadMenu.getChild("screenshot").setSurface(AGSurface.load(fn))
			end
		end
	end

	def updateSingleMenu
		puts "DIR.."
		fs=getDirectory("./data/levels")
		puts fs.join(" ")
		fs=fs.select{|f|f=~/\.antlvl/ and (not f=~/~/)}.sort.uniq
		puts fs.join(" ")
		l=@singleMenu.getChild("list")
		l.clearList
		fs.each{|f|
			x=f.gsub(".antlvl","")
			l.insertItem(f,x)
		}
		if fs.length>0
			fn="data/levels/"+fs[0].gsub("antlvl","png")
			if findFile(fn)!=""
				@singleMenu.getChild("screenshot").setSurface(AGSurface.load(fn))
			end
		end
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
		addHandler(@singleMenu.getChild("list"),:sigSelect,:eventLoadSelect)
		updateSingleMenu
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
		@optionSubMenus=["VideoOptionsMenu","AudioOptionsMenu","GameOptionsMenu"]
		@optionsMenu.loadXML(loadFile("data/gui/layout/options.xml"))
		@menues.push(@optionsMenu)
		addHandler(@optionsMenu.getChild("exit"),:sigClick,:eventExit)
		
		addHandler(@optionsMenu.getChild("fullscreen"),:sigClick,:eventFullscreen)
		["640","800","1024","1280","1400"].each{|n|
			addHandler(@optionsMenu.getChild(n),:sigClick,:eventResChange)
		}
		#addHandler(@optionsMenu.getChild("1280"),:sigClick,:eventResChange)
		addHandler(@optionsMenu.getChild("1400"),:sigClick,:eventResChange)

		addHandler(@optionsMenu.getChild("gameOptions"),:sigClick,:eventGameOptions)
		addHandler(@optionsMenu.getChild("videoOptions"),:sigClick,:eventVideoOptions)

		addHandler(@optionsMenu.getChild("optionIntro"),:sigClick,:eventOptionsChanged)

		@optionsMenu.getChild("optionIntro").setChecked(getConfig.get("intro")!="false")
	end

	def eventOptionsChanged(e)
		case e.getCaller.getName
			when "optionIntro"
				getConfig.set("intro",@optionsMenu.getChild("optionIntro").isChecked.to_s)
				#getScreen.screenshot.save("muh76.png")
		end
	end

	def eventVideoOptions
		@optionSubMenus.each{|m|@optionsMenu.getChild(m).hide}
		@optionsMenu.getChild("VideoOptionsMenu").show
	end
	def eventGameOptions
		@optionSubMenus.each{|m|@optionsMenu.getChild(m).hide}
		@optionsMenu.getChild("GameOptionsMenu").show
	end

	def eventFullscreen
		getMain.initVideo(getMain.realWidth,getMain.realHeight,32,(not getMain.fullscreen),true,1024,768)
		getConfig.set("fullscreen",getMain.fullscreen.to_s)
		return true
	end

	def eventResChange(e)
		case e.getCaller.getName
			when "640"
				setRes(640,480)
			when "800"
				setRes(800,600)
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
		getMain.initVideo(w,h,32,true,true,1024,768) #getMain.fullscreen,true)
		getConfig.set("xRes",getMain.realWidth.to_s)
		getConfig.set("yRes",getMain.realHeight.to_s)
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

	def eventTutorial(e)
		tutCampaign=Campaign.new("data/campaigns/tutorial.xml")
		soundOff
		startCampaign(tutCampaign)
		soundOn
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
		getSoundManager.stopMp3
		eventMusicEnd
	end	

	# single menu
	def eventSingleSelect(e)
		puts "MUH"
		#raise 1
		@singleMenu.getChild("desc").setText("")
		filename=id=@singleMenu.getChild("list").getSelectedID
		fn="data/levels/"+id.gsub(".antlvl",".png")
		if findFile(fn)!=""
			@singleMenu.getChild("screenshot").setSurface(AGSurface.load(fn))
		end
		doc=Document.new("data/levels/"+filename)
		d=doc.root.get("desc")
		@singleMenu.getChild("desc").setText(d)
		return true
	end

	# load menu
	def eventLoadSelect(e)
		if getMainWidget==@singleMenu
			@singleMenu.getChild("desc").setText("")
			filename=id=@singleMenu.getChild("list").getSelectedID
			fn="data/levels/"+id.gsub(".antlvl",".png")
			if findFile(fn)!=""
				s=AGSurface.load(fn)
				if s.valid
					@singleMenu.getChild("screenshot").setSurface(s)
				end
			end
			doc=Document.new("data/levels/"+filename)
			d=doc.root.get("desc")
			@singleMenu.getChild("desc").setText(d)
			return true
		end
		puts "MUH"
		#raise 1
		@loadMenu.getChild("desc").setText("")
		filename=id=@loadMenu.getChild("list").getSelectedID
		fn="savegames/"+id.gsub(".antcmp",".png")
		if findFile(fn)!=""
			s=AGSurface.load(fn)
			if s.valid
				@loadMenu.getChild("screenshot").setSurface(s)
			end
		end
		return true
	end
	def eventLoad(e)
		id=@loadMenu.getChild("list").getSelectedID
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

