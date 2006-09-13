#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# game.rb
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

#
# antargis.rb is the central file in antargis-level-playing.
# it contains the main-application object for in-game looping, etc.
# most of the GUI-events are processed here.
#


# add programdir to path
$programDir=Dir.pwd+"/ruby"
$:.push($programDir)

require 'antargislib.rb'
require 'dialogs.rb'
require 'ents.rb'
require 'map.rb'
require 'view.rb'
require 'game_result.rb'
require 'storyflow.rb'
require 'mpmap.rb'

def getSavePath
	"savegames"
end

#
# AntGameApp is the central class, that controls event handling of all the user interaction
# in-game
#
class AntGameApp <AntRubyView
	attr_accessor :result
	attr_reader :hero

	include AGHandler

	def initialize(savegameText,w,h,loadscreen=nil,connection=nil)
		super(w,h)

		# the result of the level - won or lost or canceled
		@result=GameResult.new
		$app=self
	
		# display loading screen
		if loadscreen
			loadscreen.setValue(0.1)
			loadscreen.tick
		end

		playerName=nil

		# init network mode
		@connection=connection
		if connection
			# FIXME: check, if this is a server - then don't assign playerName
			playerName=connection.getName
		end

		# init game-engine's map
		@map=AntMpMap.new(connection,getScene,32,32,playerName) # some small dummy size - gets overriden by loadMap anyway
		if loadscreen
			loadscreen.setValue(0.4)
			loadscreen.tick
		end
		$map=@map

		# load GUI layout
		@layout=AGLayout.new(nil)
		@layout.loadXML(loadFile("data/gui/layout/ant_layout.xml"))

		# init pointers to different displays
		# statusBar (FPS display)
		@statusBar=@layout.getChild("statusBar")
		@inventory=@layout.getChild("inventory")
		@buttonpanel=@layout.getChild("antButtonPanel")
		@miniMap=toMiniMap(@layout.getChild("miniMap"))
		@fps=0

		setMainWidget(@layout)
		addHandler(@layout.getChild("quit"),:sigClick,:eventQuit)
		addHandler(@layout.getChild("pause"),:sigClick,:eventPause)
		addHandler(@layout.getChild("options"),:sigClick,:eventOptions)

		if loadscreen
			loadscreen.setValue(0.5)
			loadscreen.tick
		end
		
		$screen=@layout
	
		if @miniMap
			# connect MiniMap with Map for displaying terrain and entities
			@miniMap.setMap(getMap)
			# connect MiniMap with Scene for displaying frustrum
			@miniMap.setScene(getScene)
		end

		if loadscreen
			loadscreen.setValue(0.8)
			loadscreen.tick()
		end
		
		if savegameText && savegameText.length>0
			# load a level
			getMap().loadMapFromMemory(savegameText)
		end	

		if loadscreen
			loadscreen.setValue(0.95)
			loadscreen.tick
		end


		# inventory and buttonpanel signals
		addHandler(@inventory,:sigJobChanged,:eventInventoryJob)
		addHandler(@buttonpanel,:sigJobChanged,:eventHeroJob)

		setupHeroDisplay(true)

		# infobox is used for display a text "Your hero 'xy' suffers"
		@infobox=nil

		# frame counting
		@frameCount=0
		@elapsTime=0

		setCursor(getTextureCache.get("blue_cursor.png"))
	end

	####################################
	# EVENT HANDLERS
	####################################

	# debugging-function - log all user-input
	def getNewEvent
		if false
			@eventDebugging||=File.open("events.txt","r")
			s=@eventDebugging.readline
			return toSDLEvent(s)
		end
		e=super
		s=toString(e)
		@eventDebugging||=File.open("events.txt","w")
		begin
			@eventDebugging.puts s
			puts toSDLEvent(s)
		rescue
		end
	
		return e
	end


	# this handler is for the buttonpanel on the top
	# the actions are direct ones, like rest,dimiss and so, which don't need an object to use (like fighting,taking,...)
	def eventHeroJob(e)
		case @buttonpanel.job
			when "doDismiss"
				# opens a query dialog "do really want to do this?", that is given a block, that's executed on confirmation
				@layout.addChild(AntQueryDialog.new(@layout,nil) {puts @hero;@hero.newHLDismissJob})
			when "doRest"
				if @hero
					@hero.newHLRestJob(10)
				end
			when "doBuild"
				@job="doBuild"
		end
		return true
	end

	# react to inventory-based jobs - like taking, recruiting

	def eventInventoryJob(e)
		if @target.nil? #some more overview as
			puts "NO TARGET SELECTED"
		else
			case @inventory.job
				when "doRecruit"
					@hero.newHLRecruitJob(@target)
				when "doTakeFood"
					@hero.newHLTakeFoodJob(@target)
				when "doTakeWeapons"
					@hero.newHLTakeWeaponJob(@target)
				when "doConstruct"
					@hero.newHLConstructJob(@target)
				when "doUpgrade"
					@hero.upgradeMan(@target)
				when "doContinue"
					@hero.newHLBuildJob(@target,nil)
			end
		end
		return true
	end

	def eventKeyDown(e)
		hero={	SDLK_F1=>0,
			SDLK_F2=>1,
			SDLK_F3=>2,
			SDLK_F4=>3,
			SDLK_F5=>4,
			SDLK_F6=>5,
			SDLK_F7=>6}[e.getKey]
		if hero
			h=getHeroByNum(hero) 
			if h
				if @hero==h
					focusHero(h)
				end
				selectHero(h)
				return super(e)
			end
		end
		case e.getKey
			when SDLK_F9
				if not panelVisible
					showPanel
				else
					hidePanel
				end
			when SDLK_F8
				@sceneEnabled=(not @sceneEnabled)
				getScene.setEnabled(@sceneEnabled)
			when SDLK_F1
				hero=1
		end
		return super(e)
	end
	
	def eventStoryTalkFinished
		showPanel
		getMap.trigger(nil,Trigger.new("storyFinished"))
	end

	def eventFrame(time)
		super(time)

		# FPS display
		if @frameCount>14 then
			@fps=@frameCount / @elapsTime
			fps=""
			if @fps<10
				fps=sprintf("%3.2f",@fps)
			else
				fps=sprintf("%3.0f",@fps)
			end
			puts "FPS:"+fps
			puts "pick-tris:"+getScene.getPickTriangles.to_s
			@statusBar.setText("FPS:#{fps}")
			puts "Tris:"+getScene.getTriangles.to_s
			puts "MESHES:"+getScene.getDrawnMeshes.to_s
			@frameCount=0
			@elapsTime=0
			startGC
		end
		@frameCount+=1
		@elapsTime+=time


		# move entities in game-engine
		getMap().move(time)
		# advance animations
		getScene.advance(time)
		checkHeroEnergy

		processMessages

		# save some CPU-power, if available
		if @connection
			delay(20)
		end

		if @fps>25
			delay(5)
		end
		return true
	end
	
	def eventQuit(e)
		@layout.addChild(AntQuitDialog.new(@layout))
		return true
	end
	def eventPause(e)
		if not getMap().pause then
			@layout.addChild(AntPauseDialog.new(@layout))
		else
			getMap.pause=false
		end
		return true
	end
	def eventOptions(e)
		@layout.addChild(AntOptionsDialog.new(@layout))
		return true
	end
	def eventHeroButton(e)
		c=e.getCaller.getName
		num=c[4..4]
		name=@layout.getChild("HeroName#{num}").getText
		ent=getMap.getByName(name)
		selectHero(ent)
		if ent==@inspect
			focusHero(ent)
		end
		inspectEntity(ent)
		return true

	end

	def eventAggressionChanged(e)
		h=getSelectedHero
		if h
			h.setAggression($buttonPanel.getAggression)
		end
		return true
	end

	def eventEntitiesClicked(list,button)
		resetJob
		# find first entity that's nearest to the camera
		ent=nil
		list.each{|node|
			mesh=node.node
			if [Mesh,AnimMesh].member?(mesh.class)
				ent=getMap.getEntity(mesh)
				break if ent
			end
		}

		# if an entity was found - set new target
		if ent
			if ent.class==AntHero and ent.getPlayer==getMap.getPlayer
				#@hero=ent
				@target=ent
				inspectEntity(ent)
			else
				@target=ent
				inspectEntity(ent)
			end
		end

		if button==1
			# left button == select
		elsif button==3
			# right button == fight or goto
			if ent==@target
				if @target.is_a?(AntBoss)
					if @target.getPlayer!=@hero.getPlayer
						@hero.newHLFightJob(@target)
						return
					end
				elsif @target.is_a?(AntAnimal)
					@hero.newHLFightAnimalJob(@target)
					return
				end
				# move near target
				@hero.newHLMoveJob(0,@target.getPos2D,2)
			end
		end
	end

	def eventMapClicked(pos,button)
		puts "#{@job} #{button}"
		if @job and button==1 then
			case @job
				when "doBuild"
					buildHouse(pos.dim2)
					@job=nil
			end
			resetJob
			return
		end
		if @hero and button==3 then
			# assign hero a move job
			@hero.newHLMoveJob(0,pos,0)
		end
	end


	###############################
	# simple functions
	###############################

	def processMessages
		if @connection
			while message=@connection.getMessage
				@map.processMessage(message)
			end
		end
	end

	def buildHouse(pos)
		@layout.addChild(AntBuildDialog.new(@layout,pos,@hero))
	end
	
	def save
		if $campaign
			@layout.addChild(AntSaveCampaignDialog.new(@layout))
		else
			@layout.addChild(AntSaveDialog.new(@layout))
		end
	end
	def load
		@layout.addChild(AntLoadDialog.new(@layout))
	end
	def videoOptions
		@layout.addChild(AntVideoOptionsDialog.new(@layout))
	end
	def audioOptions
		@layout.addChild(AntAudioOptionsDialog.new(@layout))
	end

	###############################################################
	# Setting up of Hero displays (images, texts and eventHandlers
	###############################################################

	def setHeroImage(image,num)
		@layout.getChild("HeroImage#{num}").setTexture(image)
	end
	def setHeroName(name,num)
		@layout.getChild("HeroName#{num}").setText(name)
		c=@layout.getChild("HeroBar#{num}")
		c.setVisible((name!=""))
	end
	def setHeroEnergy(hero,num)
		@layout.getChild("HeroBar#{num}").setHero(hero)
	end
	def setHero(flag,num)
		name="hero#{num}"
		c=@layout.getChild(name)
		c.setEnabled(flag)
		c.setVisible(flag)
	end
	def setupHeroDisplay(first=false)
		#super
		# setup Hero buttons
		hs=getMap.getPlayer.getHeroes.select{|h|h.class==AntHero}
		for i in 0..5
			if hs.length>i
				setHero(true,i)
				setHeroName(hs[i].getName,i)
				setHeroImage(hs[i].getImage,i)
				setHeroEnergy(hs[i],i)
			else
				setHero(false,i)
				setHeroImage(getTextureCache.get("data/gui/portraits/none.png"),i)
				setHeroName("",i)
			end
		end
		# init Handlers of Buttons
		if first
			addHandler(@layout.getChild("antButtonPanel"),:sigAggressionChanged,:eventAggressionChanged)
			for i in 0..2
				addHandler(@layout.getChild("hero#{i}"),:sigClick,:eventHeroButton)
			end
			setupNames
			
			# center hero and select
	 		if hs.length>0
	 			h=hs[0]
	 			selectHero(h)
 				focusHero(h)
				@layout.getChild("hero0").setChecked(true)
 			end
			
		end
		checkHeroEnergy
	end

	# updates the energy displays of the heroes (if needed)
	def checkHeroEnergy
		name=nil
		getMap.getPlayer.getHeroes.select{|h|
			if h.class==AntHero
				if h.getEnergy<0.3
					name=h.getName
				end
			end
		}
		if @infobox==nil and name!=nil
			@layout.addChild(@infobox=AntInfoBox.new(@layout,_("Your hero {1} suffers.",name)))
		elsif @infobox!=nil and name==nil
			@infobox.close
			@infobox=nil
		end
	end

	# sets a new result
	def setResult(r)
		@result=r
	end
	
	# returns boolean about the visibility of the panel
	def panelVisible
		@layout.getChild("SideBar").visible
	end

	def showPanel
		sideBar=@layout.getChild("SideBar")
		sideBarRight=@layout.getChild("SideBarRight")
		sideBar.show
		@controls=true
		# reset Scene's width
		getScene.getCameraObject.setWidth((getScreen.getWidth-sideBarRight.width).to_i)
	end
	def hidePanel
		@layout.getChild("SideBar").hide
		@controls=false
		# reset Scene's width
		getScene.getCameraObject.setWidth(getScreen.getWidth)
	end


	def tellStory(flow)
		getMap.pause=true
		hidePanel
		if @story.nil?
			@story=AntStoryTalk.new(@layout)
		end
		@layout.addChild(@story)
		@story.show
		@story.setFlow(flow)
		addHandler(@story,:sigStoryFinished,:eventStoryTalkFinished)
	end

	def inspectEntity(e)
		if @inspect
			if @inspect.is_a?(AntBoss)
				@inspect.selected=false
			end
		end
		@inspect=e
		if @inspect
			if @inspect.is_a?(AntBoss)
				@inspect.selected=true
			end
		end
		$inventory.inspect(e)
	end

	def resetJob
		#@job=nil
		@layout.getChild("doBuild").setChecked(false)
	end

	def getSelectedHero
		@hero
	end

	def selectHero(h)
		@hero=h
		inspectEntity(h)
		@buttonpanel.setName(h.getName)
		@buttonpanel.setHero(h)

		for i in 0..2
			if @layout.getChild("HeroName#{i}").getText==h.getName
				@layout.getChild("hero#{i}").setChecked(true)
			end
		end

		# set hero's aggression
		@buttonpanel.setAggression(@hero.getAggression)
	end

	def getHeroByNum(n)
		getMap.getOwnHeroes[n]
	end

	# views an information window about the entity ent
	def viewInformation(ent)
		win=AntInspectWindow.new(@layout,ent)
		@layout.addChild(win)
	end

end


# this function is only for starting a level directly (mostly for testing purpose)
def startGame(file="savegames/savegame0.antlvl",clientConnection=nil)
	disableGC
	app=nil
	if clientConnection
		app=AntGameApp.new(file,getMain.width,getMain.height,nil,clientConnection)
	else
		app=AntGameApp.new(loadFile(file),getMain.width,getMain.height,nil,clientConnection)
	end
	#app.disableGC
	app.run
	result=app.result
	# result all globals
	app=nil
	$map=nil
	$app=nil
	$screen=nil
	enableGC
	# run garbage collector
	startGC
	return result
end


# code for starting a level directly from command-line like this:
# ./ruby/antargis.rb levels/birth1
# or
# ./ruby/antargis.rb savegames/savegame0
$useMenu||=false
if true
	savegame=""
	ARGV.each{|arg|
		if arg=~/levels.*/ or arg=~/savegames.*/
			savegame=arg+".antlvl"
		end
	}
	if savegame!=""
		startGame(savegame)	
	end
end
