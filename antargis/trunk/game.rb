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
require 'sdl'

include Libantargisruby
include Libantargis

#
# Story talking window - appearance defined in storytalk.xml
#
class AntDialog<AGLayout
	include AGHandler
	def initialize(parent,filename)
		super(parent,loadFile(filename))
		addHandler(getChild("ok"),:sigClick,:sigOk)
		addHandler(getChild("cancel"),:sigClick,:sigCancel)
		#setModal(true)
	end
	def sigOk(eventName,callerName,event,caller)
		sigClose
	end
	def sigCancel(eventName,callerName,event,caller)
		sigClose
	end
	
	def eventKeyDown(event)
		puts "EVENTKEYDOWN"
		super
		puts event
	end
	def eventKeyDown(event)
		if super then return true end
		e=toAGSDLEvent(event)
		if e.getKey==SDLK_ESCAPE then	
			sigClose
			getMap.unpause
			return true
		end
	end
	def sigClose
		hide
	end
end

class AntStoryTalk<AntDialog
	include AGHandler
	def initialize(parent)
		super(parent,"storytalk.xml")
		getMap().pause()
		addHandler(toAGWindow(getChild("window")),:sigClose,:sigClose)
		#addHandler(getChild("ok"),:sigClick,:sigClose)
	end
	def setText(text)
		toAGEdit(getChild("text")).setText(text)
	end
	def setTitle(text)
		# it's AGText # FIXME: maybe exchange all AGText usages by AGEdit
		toAGText(getChild("title")).setText(text)
	end
	
	# signals	
	def sigOk(eventName,callerName,event,caller)
		puts "pCaller:"+callerName
		getMap().unpause()
		toAGWindow(getChild("window")).close
	end
end

class AntQuitDialog<AntDialog
	def initialize(parent)
		super(parent,"quitquery.xml")
		setName("QuitDialog")
	end
	def sigOk(eventName,callerName,event,caller)
		$app.tryQuit
	end
end

class AntOptionsDialog<AntDialog
	def initialize(parent)
		super(parent,"optionsmenu.xml")
		setName("OptionsDialog")
		addHandler(getChild("editmode"),:sigClick,:sigEditmode)
		addHandler(getChild("save"),:sigClick,:sigSave)
		addHandler(getChild("load"),:sigClick,:sigLoad)
		getMap.pause
	end
	def sigEditmode(ename,cname,event,caller)
		$app.enableEdit
	end
	def sigSave(ename,cname,event,caller)
		$app.save
		hide
	end
	def sigLoad(ename,cname,event,caller)
		$app.load
		hide
	end
end

class AntSaveDialog<AntDialog
	def initialize(parent)
		super(parent,"savedialog.xml")
	end
	def sigOk(eventName,callerName,event,caller)
		filename=toAGEdit(getChild("Filename")).getText
		puts "FILENAME:"+filename
		getMap.saveMap(filename)
		getMap.unpause
		hide
	end
end

class AntPauseDialog<AntDialog
	def initialize(parent)
		super(parent,"pause.xml")
		setName("PauseDialog")
		getMap.pause
	end
	def sigOk(eventName,callerName,event,caller)
		getMap.unpause
		hide
	end
end

class AntGameApp <AntApp
	def initialize()
		super()
		$app=self	
		@map=AntargisMap.new(128,128)	
		# load a level
		getMap().loadMap("dummy.antlvl")
	
		@layout=AGLayout.new(nil,loadFile("ant_layout.xml"))
		setMainWidget(@layout)
		addHandler(@layout.getChild("quit"),:sigClick,:sigQuit)
		addHandler(@layout.getChild("pause"),:sigClick,:sigPause)
		addHandler(@layout.getChild("options"),:sigClick,:sigOptions)
		
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
		#GC.start
		return true
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
		@layout.addChild(AntSaveDialog.new(@layout))
	end
end

main=AGMain.new

main.changeRes(1024,768,32,false,true)

app=AntGameApp.new()

app.run

