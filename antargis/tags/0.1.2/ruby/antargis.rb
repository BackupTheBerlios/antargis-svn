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
	attr_reader :hero

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
		puts "MAPPPPPPPPPPPPPP:#{@map}"

		@finish="quit"

		#GC.disable
		@layout=AGLayout.new(nil)
		@layout.loadXML(loadFile("data/gui/layout/ant_layout.xml"))

		@statusBar=@layout.getChild("statusBar")

		c=@layout.getChild("inventory")
		puts "inventory:"
		puts c

		c=@layout.getChild("HeroBar0")
		puts "HeroBar:",@layout
		puts c

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
		@miniMap=toMiniMap(@layout.getChild("miniMap"))
		if @miniMap
			@miniMap.setMap(getMap)
			@miniMap.setScene(getScene)
		end
		if loadscreen
			loadscreen.setValue(0.7)
			loadscreen.tick
		end
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


		# inventory and buttonpanel signals
		@inventory=@layout.getChild("inventory")
		addHandler(@inventory,:sigJobChanged,:eventInvJob)
		@buttonpanel=@layout.getChild("antButtonPanel")
		addHandler(@buttonpanel,:sigJobChanged,:eventHeroJob)

		setupHeroDisplay(true)

		@infobox=nil

	end

	def eventHeroJob(e)
		puts "eventHeroJob"
		puts @buttonpanel.job
		case @buttonpanel.job
			when "doDismiss"
				@layout.addChild(AntQueryDialog.new(@layout,nil) {puts @hero;@hero.newHLDismissJob})
				puts @hero
				#@hero.newHLDismissJob
			when "doRest"
				#messageBox("restTitle","rest text",0) {|result|puts "RESULT:#{result}"}
				#getMap.pause
				if @hero
					@hero.newHLRestJob(10)
				end
		end
		return true
	end

	def eventInvJob(e)
		puts "INVJOBBBBBBBBBBBBBBBBB"
		if @target==nil
			puts "NO TARGET SELECTED"
			return true
		end
		puts @inventory.job
		case @inventory.job
			when "doRecruit"
				@hero.newHLRecruitJob(@target)
			when "doTakeFood"
				@hero.newHLTakeFoodJob(@target)
			when "doTakeWeapons"
				@hero.newHLTakeWeaponJob(@target)
			when "doConstruct"
				@hero.newHLConstructJob(@target)
		end
		return true
	end

	def eventKeyDown(e)
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
		end
		return super(e)
	end
	
	def setResult(r)
		@result=r
	end
	
	def initDebug
		@debug=AGLayout.new(@layout)
		@debug.loadXML(loadFile("debug.xml"))
		@layout.addChild(@debug)
		addHandler(@debug.getChild("load"),:sigClick,:load)
	end

	def panelVisible
		puts @layout.getChild("SideBar").visible
		@layout.getChild("SideBar").visible
	end
	def showPanel
		puts "SHOWPANEL"
		sideBar=@layout.getChild("SideBar")
		sideBar.show
		@controls=true
		getScene.getCameraObject.setWidth((getScreen.getWidth-sideBar.width).to_i)
	end
	def hidePanel
		puts "HIDEPANEL"
		@layout.getChild("SideBar").hide
		@controls=false
		getScene.getCameraObject.setWidth(getScreen.getWidth)
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
		#puts "eventFrame"
		super(time)
		#puts "eventFrame2"
		if @fc==nil then 
			@fc=0 
			@elaps=0
		end
		#puts "eventFrame3"
		if @fc>14 then
			@fps=@fc/@elaps
			fps=sprintf("%3.0f",@fps)
			puts "FPS:"+fps
			@statusBar.setText("FPS:#{fps}")
			puts "Tris:"+getScene.getTriangles.to_s
			@fc=0
			@elaps=0
			startGC
		end
		@fc+=1
		@elaps+=time
		getMap().move(time)
		getScene.advance(time)
		checkHeroEnergy

		if @fps>25
			delay(5)
		end
		return true
	end
	
	# signals	
	def eventQuit(e)
		@layout.addChild(AntQuitDialog.new(@layout))
		return true
	end
	def eventPause(e)
		if not getMap().paused then
			@layout.addChild(AntPauseDialog.new(@layout))
		else
			getMap().unpause
		end
		return true
	end
	def eventOptions(e)
		@layout.addChild(AntOptionsDialog.new(@layout))
		return true
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
		puts "NAME:",name
		c=@layout.getChild(name)
		puts c
		c.setEnabled(flag)
		puts "setHero-ok"
	end
	def setupHeroDisplay(first=false)
		#super
		# setup Hero buttons
		hs=getMap.getPlayer.getHeroes.select{|h|h.class==AntHero}
		for i in 0..2
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

	def checkHeroEnergy
		name=nil
		getMap.getPlayer.getHeroes.select{|h|
			if h.class==AntHero
				#puts h.getEnergy
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
	def viewInformation(ent)
		win=AntInspectWindow.new(@layout,ent) #.getDescription)
		@layout.addChild(win)
	end
	def clickEntities(list,button)
		puts "clickEntities"
		puts button

		# find first entity
		ent=nil
		list.each{|node|
			mesh=node.node
			if [Mesh,AnimMesh].member?(mesh.class)
				ent=getMap.getEntity(mesh)
				break if ent
			end
		}
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
			# right button == fight
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

	def clickMap(pos,button)
		puts "CLICKMAP"
		if @hero and button==3 then
			@hero.newHLMoveJob(0,pos,0)
		end
	end

end

def startGame(file="savegames/savegame0.antlvl")
	disableGC
	app=AntGameApp.new(file,getMain.width,getMain.height)
	#app.disableGC
	app.run
	result=app.result
	app=nil
	$map=nil
	$app=nil
	$screen=nil
	puts "GC RUN"
	#app.enableGC
	enableGC
	startGC
	return result
end
# def frustumTest
# 	puts "frustumTest"
# 	p=AntPlane.new(AGVector3.new(1,1,-1).normalized,1)
# 	
# 	puts p.inside(AGVector3.new(2,5,6))
# 	
# 	#exit
# end
# frustumTest

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
if false
if not $useMenu and (ENV["_"]=~/antargis.rb/ or ENV["_"]=~/bash/ or ENV["_"]=~/gdb/)
	savegame="levels/level1.antlvl"
	ARGV.each{|arg|
		if arg=~/levels.*/ or arg=~/savegames.*/
			savegame=arg+".antlvl"
		end
	}
	puts "LOADING:"+savegame
	startGame(savegame)	
end



end