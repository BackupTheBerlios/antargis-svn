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

#require 'libantargisruby'
require 'libantargis'
require 'antApp.rb'
#require 'sdl'

#include Libantargisruby
include Libantargis

require 'dialogs.rb'
require 'ents.rb'
require 'map.rb'
require 'view.rb'

class AntGameApp <AntApp
	def initialize()
		super()
		$app=self	
#		@map=AntargisMap.new(128,128)	
		@map=AntRubyMap.new(128,128)	
		$map=@map
		# load a level
		getMap().loadMap("savegames/savegame6.antlvl")
	
		#@view=AntRubyView.new(nil,AGRect.new(0,0,getMain().width,getMain.height),Pos3D.new(0,0,0))
		#puts @view
		@layout=AGLayout.new(nil,loadFile("ant_layout.xml"))
		setMainWidget(@layout)
		addHandler(@layout.getChild("quit"),:sigClick,:sigQuit)
		addHandler(@layout.getChild("pause"),:sigClick,:sigPause)
		addHandler(@layout.getChild("options"),:sigClick,:sigOptions)
		
		puts @layout.getChild("mainView").getName
		$screen=@layout
		
#		initDebug
		
		#storyTalk("Welcome","Welcome to Battles of Antargis")
		#test
#		getMap.endChange
	end
	
	def initDebug
		@debug=AGLayout.new(@layout,loadFile("debug.xml"))
		@layout.addChild(@debug)
		addHandler(@debug.getChild("load"),:sigClick,:load)
		addHandler(@debug.getChild("test1"),:sigClick,:test)
		addHandler(@debug.getChild("save"),:sigClick,:testsave)
	end
	
	def storyTalk(title,text)
		@story=AntStoryTalk.new(@layout)
		@layout.addChild(@story)
		@story.setText(text)
		@story.setTitle(title)
	end

	def eventFrame(time)
		if $fc==nil then $fc=0 end
		if $fc>10 then
			puts "FPS:"+(1.0/time).to_s
			$fc=0
		end
		$fc+=1
		getMap().move(time)
		#GC.start
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
	
	def test
		puts "TESTTTTTTTTTTTTTTTTTTTTTTTTT"
		getMap.clear
		#ent=AntMan.new(Pos2D.new(30,230),0,nil)
		(0..10).each{
		ent=AntNewMan.new
		ent.setPos2D(Pos2D.new(500,530))
		$map.insertEntity(ent)
		}
		
		ent=AntNewTree.new
		ent.setPos2D(Pos2D.new(400,400))
		$map.insertEntity(ent)
		
		puts ent
		ent2=AntNewHouse.new
		ent2.setPos2D(Pos2D.new(600,600))
		$map.insertEntity(ent2)
		puts "GC"
		GC.start
		
		getMap.endChange
	end
	def testsave
		getMap.saveMap("savegame2")
	end
	
end

main=AGMain.new

main.changeRes(1024,768,32,false,true)
#main.changeRes(1400,1050,32,true,true)

app=AntGameApp.new()

app.run

