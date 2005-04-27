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

require 'libantargisruby'
require 'libantargis'
require 'antApp.rb'

include Libantargisruby
include Libantargis


class AntStoryTalk<AGLayout
	include AGHandler
	def initialize(parent)
		super(parent,loadFile("storytalk.xml"))
		getMap().pause()
		addHandler(toAGWindow(getChild("window")),:sigClose,:sigClose)
		addHandler(getChild("ok"),:sigClick,:sigClose)
	end
	def setText(text)
		toAGEdit(getChild("text")).setText(text)
	end
	def setTitle(text)
		# it's AGText # FIXME: maybe exchange all AGText usages by AGEdit
		toAGText(getChild("title")).setText(text)
	end
	# signals	
	def sigClose(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		getMap().unpause()
		toAGWindow(getChild("window")).close
	end
end

class AntGameApp <AntApp
	def initialize()
		super()
	
		@map=AntargisMap.new(128,128)	
		# load a level
		getMap().loadMap("dummy.antlvl")
	
		@layout=AGLayout.new(nil,loadFile("ant_layout.xml"))
		setMainWidget(@layout)
		addHandler(@layout.getChild("quit"),:sigClick,:sigQuit)
		addHandler(@layout.getChild("pause"),:sigClick,:sigPause)
		
		storyTalk("Welcome","Welcome to Battles of Antargis")
	end
	
	def storyTalk(title,text)
		@story=AntStoryTalk.new(@layout)
		@layout.addChild(@story)
		@story.setText(text)
		@story.setTitle(title)
	end
	

	def eventFrame(time)
		getMap().move(time)
		return true
	end
	
	# signals	
	def sigQuit(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		tryQuit
	end
	# signals	
	def sigPause(eventName,callerName,event,caller)
		getMap.pause
		#puts "pCaller:"+callerName
		#tryQuit
	end
end

main=AGMain.new

main.changeRes(1024,768,32,false,true)

app=AntGameApp.new()

app.run

