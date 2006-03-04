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

#$debug=true

$programDir=Dir.pwd+"/ruby"
# add programdir to path
$:.push($programDir)

require 'antargislib.rb'

require 'dialogs.rb'
require 'ents.rb'
require 'map.rb'
require 'view.rb'
require 'game_result.rb'
require 'storyflow.rb'

class AntGameApp <AntRubyView
	attr_accessor :result


	include AGHandler
	def initialize(savegame,w,h,loadscreen=nil)
		super(w,h)
		@result=GameResult.new
		$app=self	
		if loadscreen
			loadscreen.setValue(0.1)
			loadscreen.tick
		end
		@map=AntRubyMap.new(getScene,32,32) # some small dummy size - gets overriden by loadMap anyway
		if loadscreen
			loadscreen.setValue(0.4)
			loadscreen.tick
		end
		$map=@map
		@finish="quit"

		@layout=AGLayout.new(nil,loadFile("data/gui/layout/ant_layout.xml"))
		setMainWidget(@layout)
		addHandler(@layout.getChild("quit"),:sigClick,:eventQuit)
		addHandler(@layout.getChild("pause"),:sigClick,:eventPause)
		addHandler(@layout.getChild("options"),:sigClick,:eventOptions)
		if loadscreen
			loadscreen.setValue(0.5)
			loadscreen.tick
		end
		
		$screen=@layout
		@miniMap=toMiniMap(@layout.getChild("miniMap"))
		@miniMap.setMap(getMap)
		if loadscreen
			loadscreen.setValue(0.7)
			loadscreen.tick
		end
		@miniMap.setScene(getScene)
		if loadscreen
			loadscreen.setValue(0.8)
			loadscreen.tick
		end
		
		if savegame && savegame.length>0
			# load a level
			getMap().loadMap(savegame)
		end	
		if loadscreen
			loadscreen.setValue(0.95)
			loadscreen.tick
		end

		#@miniMap.mapChangedComplete(AGEvent.new(self,"bla"))
		setupHeroDisplay(true)

	end

	def eventKeyDown(e)
		if e.getKey==SDLK_F9
			if @layout.visible
				@layout.hide
			else
				@layout.show
			end
		end
		return super(e)
	end
	
	def setResult(r)
		@result=r
	end
	
	def initDebug
		@debug=AGLayout.new(@layout,loadFile("debug.xml"))
		@layout.addChild(@debug)
		addHandler(@debug.getChild("load"),:sigClick,:load)
	end
	
	def showPanel
		@layout.getChild("SideBar").show
	end
	def hidePanel
		@layout.getChild("SideBar").hide
	end


	def storyTalk(flow)
		hidePanel
		@story=AntStoryTalk.new(@layout)
		@layout.addChild(@story)
		@story.setFlow(flow)
		addHandler(@story,:sigStoryFinished,:storyTalkFinished)
	end

	def storyTalkFinished
		showPanel
		getMap.trigger(nil,Trigger.new("storyFinished"))
	end

	def eventFrame(time)
		super(time)
		if @fc==nil then 
			@fc=0 
			@elaps=0
		end
		if @fc>30 then
			puts "FPS:"+(@fc/@elaps).to_s
			puts "Tris:"+getScene.getTriangles.to_s
			@fc=0
			@elaps=0
		end
		@fc+=1
		@elaps+=time
		getMap().move(time)
#		GC.start
		getScene.advance(time)
		delay(10)
		return true
	end
	
	def eventIdle
		#GC.start
	end
	
	# signals	
	def eventQuit(e)
		#puts "pCaller:"+callerName
		#tryQuit
		@layout.addChild(AntQuitDialog.new(@layout))
	end
	def eventPause(e)
		if not getMap().paused then
			@layout.addChild(AntPauseDialog.new(@layout))
		else
			getMap().unpause
		end
	end
	def eventOptions(e)
		@layout.addChild(AntOptionsDialog.new(@layout))
	end
	
	def enableEdit
		toEditIsoView(@layout.getChild("mainView")).toggleEdit
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
	def finished
		@finish
	end
	def videoOptions
		@layout.addChild(AntVideoOptionsDialog.new(@layout))
	end
	def selectHero(h)
	end

	def setHeroImage(image,num)
		@layout.getChild("HeroImage#{num}").setTexture(image)
	end
	def setHeroName(name,num)
		@layout.getChild("HeroName#{num}").setText(name)
	end
	def setHero(flag,num)
		name="hero#{num}"
		puts "NAME:",name
		c=@layout.getChild(name)
		puts c
		c.setEnabled(flag)
		return
		if flag
			c.show
		else
			c.hide
		end
	end
	def setupHeroDisplay(first=false)
		super
		hs=getMap.getPlayer.getHeroes.select{|h|h.class==AntHero}
		puts "HEROOOOOOOOOOOOOOOOOOOOOOOES:",hs
		for i in 0..2
			if hs.length>i
				setHero(true,i)
				setHeroName(hs[i].getName,i)
				setHeroImage(hs[i].getImage,i)
			else
				setHero(false,i)
				setHeroImage(getTextureCache.get("data/gui/portraits/none.png"),i)
				setHeroName("",i)
			end
		end

		# init
		if first
			addHandler(@layout.getChild("antButtonPanel"),:sigAggressionChanged,:eventAggressionChanged)
			for i in 0..2
				addHandler(@layout.getChild("hero#{i}"),:sigClick,:eventHeroButton)
			end
		end
	end

	def eventHeroButton(e)
		c=e.getCaller.getName
		num=c[4..4]
		name=@layout.getChild("HeroName#{num}").getText
		ent=getMap.getByName(name)
		if ent==@inspect
			focusHero(ent)
		end
		inspectEntity(ent)
		
	end

	def eventAggressionChanged(e)
		h=getSelectedHero
		if h
			h.setAggression($buttonPanel.getAggression)
		end
	end
	
	def getSelectedHero
		@hero
	end

	def selectHero(h)
		@hero=h
		inspectEntity(h)

		for i in 0..2
			if @layout.getChild("HeroName#{i}").getText==h.getName
				@layout.getChild("hero#{i}").setChecked(true)
			end
		end
	end
	def viewInformation(ent)
		win=AntInspectWindow.new(@layout,ent) #.getDescription)
		@layout.addChild(win)
	end
end

def startGame(file="savegames/savegame0.antlvl")
	app=AntGameApp.new(file,getMain.width,getMain.height)
	app.run
	result=app.result
	app=nil
	$map=nil
	$app=nil
	$screen=nil
	puts "GC RUN"
	GC.start
	return result
end

if $useMenu==nil and (ENV["_"]=~/antargis.rb/ or ENV["_"]=~/bash/ or ENV["_"]=~/gdb/)
	savegame="levels/level1.antlvl"
	if ARGV.length>0
		if ARGV[0]=~/\//
			savegame=ARGV[0]+".antlvl"
		else
			savegame="savegames/"+ARGV[0]+".antlvl"  # was levels/
		end
	end
	puts "LOADING:"+savegame
	startGame(savegame)	
end

