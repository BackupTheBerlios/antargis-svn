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

$programDir=Dir.pwd+"/ruby"
# add programdir to path
$:.push($programDir)

require 'antargislib.rb'

require 'dialogs.rb'
require 'ents.rb'
require 'map.rb'
require 'view.rb'

class AntGameApp <AntRubyView
	include AGHandler
	def initialize(savegame,w,h)
		super(w,h)
		$app=self	
		@map=AntRubyMap.new(64,64)
		$map=@map
		@map.setScene(getScene)

		if savegame && savegame.length>0
			# load a level
			getMap().loadMap(savegame)
		end	
		@layout=AGLayout.new(nil,loadFile("data/gui/layout/ant_layout.xml"))
		setMainWidget(@layout)
		addHandler(@layout.getChild("quit"),:sigClick,:sigQuit)
		addHandler(@layout.getChild("pause"),:sigClick,:sigPause)
		addHandler(@layout.getChild("options"),:sigClick,:sigOptions)
		
		$screen=@layout
		
		setupHeroDisplay
	end
	
	def setupHeroDisplay
		all=getMap.getAllEntitiesV
		all.each{|ep|
			e=ep.get
			if e.getType=="hero"
				puts e.getName
			end
		}
		#exit
	end
	
	
	
	def initDebug
		@debug=AGLayout.new(@layout,loadFile("debug.xml"))
		@layout.addChild(@debug)
		addHandler(@debug.getChild("load"),:sigClick,:load)
	end
	
	def storyTalk(title,text)
		@story=AntStoryTalk.new(@layout)
		@layout.addChild(@story)
		@story.setText(text)
		@story.setTitle(title)
	end

	def eventFrame(time)
		if $fc==nil then 
			$fc=0 
			$elaps=0
		end
		if $fc>30 then
			puts "FPS:"+($fc/$elaps).to_s
			$fc=0
			$elaps=0
		end
		$fc+=1
		$elaps+=time
		getMap().move(time)
		GC.start
		getScene.advance(time)
		delay(20)
		return true
	end
	
	def eventIdle
		#GC.start
	end
	
	# signals	
	def sigQuit(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		#tryQuit
		@layout.addChild(AntQuitDialog.new(@layout))
	end
	def sigPause(eventName,callerName,event,caller)
		if not getMap().paused then
			@layout.addChild(AntPauseDialog.new(@layout))
		else
			getMap().unpause
		end
	end
	def sigOptions(ename,cname,ev,c)
		@layout.addChild(AntOptionsDialog.new(@layout))
	end
	
	def enableEdit
		toEditIsoView(@layout.getChild("mainView")).toggleEdit
	end
	def save
		@layout.addChild(AntSaveDialog.new(@layout))
	end
	def load
		@layout.addChild(AntLoadDialog.new(@layout))
	end
	
end

main=AGMain.new(1024,768,32,false,false)

main.changeRes(1024,768,32,false,true)
#main.changeRes(1400,1050,32,true,true)

savegame="savegames/savegame0.antlvl"
if ARGV.length>0
	savegame="savegames/"+ARGV[0]+".antlvl"
end

app=AntGameApp.new(savegame,1024,768)

app.run

puts "huhu"

