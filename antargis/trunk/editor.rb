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
require 'editview.rb'

class AntGameApp < AntRubyEditView
#AntApp
	def initialize(sw,sh)
		super(sw,sh,AntRubyMap.new(64,64))
		$app=self	
#		@map=AntargisMap.new(128,128)	
		#@map=AntRubyMap.new(128,128)	
		#$map=@map
		#return
		# load a level
		#getMap().loadMap("savegames/savegame6.antlvl")
	
		#@view=AntRubyView.new(nil,AGRect.new(0,0,getMain().width,getMain.height),Pos3D.new(0,0,0))
		#puts @view
		@layout=AGLayout.new(@mainWidget,loadFile("data/gui/layout/editor.xml"))
		@mainWidget.addChild(@layout)
		#$screen=@layout
		addHandler(@layout.getChild("quit"),:sigClick,:sigQuit)
		addHandler(@layout.getChild("pause"),:sigClick,:sigPause)
		addHandler(@layout.getChild("options"),:sigClick,:sigOptions)
		
		addHandler(@layout.getChild("doGen"),:sigClick,:sigGenerate)
		
		puts "LAYOUTNAME:"
		puts @layout.getName
		#puts @layout.getChild("mainView").getName
		$screen=@layout
		
#		initDebug
		
		#storyTalk("Welcome","Welcome to Battles of Antargis")
		#test
#		getMap.endChange
 		#@mainWidget=@layout
	end
	
	
	def sigGenerate
		map=getMap
		map.setAllWater
		
		w=map.width
		h=map.height
		
		flat=[[15,9],[10,8],[5,40],[4,30],[3,70],[2,70],[1,70]]
		flat.each{|add|
			for i in 1..add[1]
				rad=add[0]
				x=rand*(w-2*rad)+rad
				y=rand*(h-2*rad)+rad
				map.addFlat(x,y,30,rad)
			end
		}
		
		
	end
	
	#def initDebug
	#	@debug=AGLayout.new(@layout,loadFile("debug.xml"))
	#	@layout.addChild(@debug)
	#	addHandler(@debug.getChild("load"),:sigClick,:load)
	#	addHandler(@debug.getChild("test1"),:sigClick,:test)
	#	addHandler(@debug.getChild("save"),:sigClick,:testsave)
	#end
	
	##def storyTalk(title,text)
	#	@story=AntStoryTalk.new(@layout)
	#	@layout.addChild(@story)
	#	@story.setText(text)
	#	@story.setTitle(title)
	#end

	def eventFrame(time)
		super(time)
		# prevent view from updating each frame
		#if getMap.updated or getMap.heightChanged
		#	getMap.move(0)
		#end
		return
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
		return true
	end
	def eventFrameEnd(time)
		#puts "EVENT FRAME END"
		#getMap.frameEnd
	end
	
	def eventIdle
		delay(20)
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

main=AGMain.new

main.changeRes(1024,768,32,false,true)
#main.changeRes(1400,1050,32,true,true)

app=AntGameApp.new(1024,768)

app.run

