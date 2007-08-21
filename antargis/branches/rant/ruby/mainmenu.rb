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

require 'ruby/antargislib.rb'
$useMenu=true

require "antargis.rb"
require 'campaign.rb'
require 'credits.rb'

# AntMenuApp implements the whole menu-block of BoA
# It uses the setMainWidget-function of AGApplication
# This way the current menu can easily be exchanged.

class AntMenuApp <AntApplication
	include AGHandler
	def initialize
		super()
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

	def setMainWidget(w)
		if @myScreen.nil?
			@myScreen||=AGScreenWidget.new
			super(@myScreen)
		end
		@myScreen.removeChild(@curMainWidget)
		@myScreen.addChild(w)
		@curMainWidget=w
	end
	
	def eventMusicEnd
		if @sound
			getSoundManager.playMp3("data/music/calm1.ogg")
		end
		return true
	end


	# MAIN MENU
	def setupMain()
		@mainMenu=AGLayout.new(nil)
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



	# LOAD GAME MENU
	def setupLoadMenu
		@loadMenu=AGLayout.new(nil)
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
			l.insertItem(f,AGStringUtf8.new(x))  # FIXME: _() ??
		}
		if fs.length>0
			fn="savegames/"+fs[0].gsub("antcmp","png")
			if findFile(fn)!=""
				@loadMenu.getChild("screenshot").setSurface(AGSurface.load(fn))
			end
		end
	end

	def eventLoadSelect(e)
		if getMainWidget==@singleMenu
			@singleMenu.getChild("desc").setText(AGStringUtf8.new(""))
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
			@singleMenu.getChild("desc").setText(AGStringUtf8.new(d))
			return true
		end
		puts "MUH"
		@loadMenu.getChild("desc").setText(AGStringUtf8.new(""))
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


	# SINGLE GAME MENU
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
			l.insertItem(f,AGStringUtf8.new(x))
		}
		if fs.length>0
			fn="data/levels/"+fs[0].gsub("antlvl","png")
			if findFile(fn)!=""
				@singleMenu.getChild("screenshot").setSurface(AGSurface.load(fn))
			end
		end
	end
	def setupSingle
		@singleMenu=AGLayout.new(nil)
		@singleMenu.loadXML(loadFile("data/gui/layout/single.xml"))
		@menues.push(@singleMenu)
		addHandler(@singleMenu.getChild("singleExit"),:sigClick,:eventExit)
		addHandler(@singleMenu.getChild("singleStart"),:sigClick,:eventSingleStart)
		addHandler(@singleMenu.getChild("list"),:sigSelect,:eventLoadSelect)
		updateSingleMenu
	end
	
	def eventSingleStart(e)

		id=@singleMenu.getChild("list").getSelectedID
		if id!=""
			soundOff
			#c=Campaign.new(getWriteDir+"/savegames/"+id)
			startGame("data/levels/"+id) #ampaign(c)
			soundOn
		end
		setMainWidget(@mainMenu)
		return true

	end




	# CREDITS MENU
	def setupCredits
		@creditsMenu=AGLayout.new(nil)
		@creditsMenu.loadXML(loadFile("data/gui/layout/credits.xml"))
		@menues.push(@creditsMenu)
		addHandler(@creditsMenu.getChild("exit"),:sigClick,:eventExit)

		@creditsMenu.getChild("ticker").menuapp=self
	end

	# CAMPAIGN MENU	
	def setupCampaign
		@campaignMenu=AGLayout.new(nil)
		@campaignMenu.loadXML(loadFile("data/gui/layout/campaign.xml"))
		@menues.push(@campaignMenu)
		addHandler(@campaignMenu.getChild("exit"),:sigClick,:eventExit)
		addHandler(@campaignMenu.getChild("start"),:sigClick,:eventStart)
		
		@campaigns=getCampaigns
		
		buttonCount=6
		campaignButtons=(0..(buttonCount-1)).to_a.collect{|c|"campaign#{c}"}
		i=0
		campaignButtons.each{|b|
			puts "get widget named: #{b}"
			c=@campaignMenu.getChild(b)
			puts "got widget #{c} of type #{c.class}"
			addHandler(c,:sigClick,:eventMission)
			if @campaigns.length>i
				c.setCaption(_(@campaigns[i].name))
			else
				c.hide
			end
			i+=1
		}
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
	

	# OPTIONS MENU
	def setupOptions
		@optionsMenu=AGLayout.new(nil)
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

		# texture quality
		texQ=@optionsMenu.getChild("textureQuality")
		texQ.setSelected({"1"=>"high","2"=>"medium","3"=>"low",nil=>"high",""=>"high"}[getConfig.get("terrainDownScaleExp")])
		addHandler(texQ,:sigSelect,:eventTextureQuality)

		# FBO
		@optionsMenu.getChild("UseFBO").setChecked(getConfig.get("useFBO")=="true")
		addHandler(@optionsMenu.getChild("UseFBO"),:sigClick,:eventOptionsChanged)
		
		# Anim
		@optionsMenu.getChild("UseAnim").setChecked(getConfig.get("animationType")=="true")
		addHandler(@optionsMenu.getChild("UseAnim"),:sigClick,:eventOptionsChanged)

		# particle
		@optionsMenu.getChild("UseParticle").setChecked(getConfig.get("particleEffects")=="true")
		addHandler(@optionsMenu.getChild("UseParticle"),:sigClick,:eventOptionsChanged)
	end
	
	def eventTextureQuality(e)
		scale={"low"=>3,"medium"=>2,"high"=>1}[@optionsMenu.getChild("textureQuality").getSelected]
		getConfig.set("terrainDownScaleExp",scale.to_s)
		getConfig.set("meshDownScaleExp",scale.to_s)
		return true
	end

	def eventOptionsChanged(e)
		case e.getCaller.getName
			when "optionIntro"
				getConfig.set("intro",@optionsMenu.getChild("optionIntro").isChecked.to_s)
				#getScreen.screenshot.save("muh76.png")
			when "UseFBO"
				getConfig.set("useFBO",@optionsMenu.getChild("UseFBO").isChecked.to_s)
			when "UseAnim"
				getConfig.set("animationType",@optionsMenu.getChild("UseAnim").isChecked.to_s)
			when "UseParticle"
				getConfig.set("particleEffects",@optionsMenu.getChild("UseParticle").isChecked.to_s)
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
		if @frames>100
			puts "FPS:#{100.0/@frameTime}"
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

	# load menu
end

app=AntMenuApp.new

app.run

