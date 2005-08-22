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
	def sigOk
		sigClose
	end
	def sigCancel(eventName,callerName,event,caller)
		sigClose
	end
	
	def eventKeyDown(event)
		if super then return true end
		e=toAGSDLEvent(event)
		if e.getKey==SDLK_ESCAPE then	
			sigClose
			getMap.unpause
			return true
		elsif e.getKey==SDLK_RETURN then
			sigOk
		end
	end
	def sigClose
		hide
	end
end

class AntStoryTalk<AntDialog
	include AGHandler
	def initialize(parent)
		super(parent,"data/gui/layout/storytalk.xml")
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
	def sigOk
		puts "pCaller:"+callerName
		getMap().unpause()
		toAGWindow(getChild("window")).close
	end
end

class AntQuitDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/quitquery.xml")
		setName("QuitDialog")
	end
	def sigOk
		$app.tryQuit
	end
end

class AntOptionsDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/optionsmenu.xml")
		setName("OptionsDialog")
		addHandler(getChild("editmode"),:sigClick,:sigEditmode)
		addHandler(getChild("save"),:sigClick,:sigSave)
		addHandler(getChild("load"),:sigClick,:sigLoad)
		getMap.pause
	end
	def sigEditmode
		$app.enableEdit
	end
	def sigSave
		$app.save
		hide
	end
	def sigLoad
		$app.load
		hide
	end
	def sigOk
		getMap.unpause
		super
	end
end

class AntSaveDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/savedialog.xml")
	end
	def sigOk
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
		super(parent,"data/gui/layout/loaddialog.xml")
		d=getDirectory(toAGEdit(getChild("Directory")).getText)
		@lb=toAGListBox(getChild("Files"))
		puts d
		d.each { |x|
			if x =~ /.*\.antlvl/ then
				puts "FILE:"+x
				value=x.gsub(".antlvl","")
				@lb.insertItem(x,value)
			end
		}
	end
	def sigOk
		file=@lb.getSelectedID
		if file!="" then
			getMap.loadMap("savegames/"+file)
			GC.start
		end
		getMap.unpause
		hide
	end
end

class AntPauseDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/pause.xml")
		setName("PauseDialog")
		getMap.pause
	end
	def sigOk
		getMap.unpause
		hide
	end
end

class AntEditPropDialog<AntDialog
	def initialize(parent,ent)
		super(parent,"data/gui/layout/editpropdialog.xml")
		@ent=ent
		menCountW=toAGEdit(getChild("MenCount"))
		nameW=toAGEdit(getChild("AntName"))
		menCountW.setText(@ent.getXMLProp("men"))
		nameW.setText(@ent.getName)
		
	end
	def sigOk
		menCountW=toAGEdit(getChild("MenCount"))
		nameW=toAGEdit(getChild("AntName"))
		@ent.setName(nameW.getText)
		@ent.setXMLProp("men",menCountW.getText)
		hide
	end
end