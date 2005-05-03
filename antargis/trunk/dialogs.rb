#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# dialogs.rb
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
		elsif e.getKey==SDLK_RETURN then
			sigOk("","",event,self)
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
		addHandler(toAGWindow(getChild("window")),:sigClose,:sigOk)
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
		if not filename =~ /.*\.antlvl/ then
			filename=filename+".antlvl"
		end
		getMap.saveMap("savegames/"+filename)
		getMap.unpause
		hide
	end
end

class AntLoadDialog<AntDialog
	def initialize(parent)
		super(parent,"loaddialog.xml")
		d=getDirectory(toAGEdit(getChild("Directory")).getText)
		@lb=toAGListBox(getChild("Files"))
		puts d
		d.each { |x|
			if x =~ /.*\.antlvl/ then
				puts "FILE:"+x
				value=x.gsub(".antlvl","")
				@lb.insert(x,value)
			end
		}
	end
	def sigOk(ename,cname,event,caller)
		file=@lb.getSelectedID
		if file!="" then
			getMap.loadMap("savegames/"+file)
		end
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
