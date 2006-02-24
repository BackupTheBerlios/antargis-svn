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
		addHandler(getChild("ok"),:sigClick,:eventOk)
		if getChild("cancel")
			addHandler(getChild("cancel"),:sigClick,:eventCancel)
		end
		addSignal("sigClosed")
		#setModal(true)
	end
	def eventOk(e)
		eventClose
	end
	def eventCancel
		eventClose
	end
	
	def eventKeyDown(e)
		if super then return true end
		if e.getKey==SDLK_ESCAPE then	
			eventClose
			getMap.unpause
			return true
		elsif e.getKey==SDLK_RETURN then
			eventOk(e)
		end
	end
	def eventClose
		hide
		sigClosed(AGEvent.new(self,"sigClosed"))
	end
end

class AntStoryTalk<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/storytalk.xml")
		getMap().pause()
		addHandler(getChild("window"),:sigClose,:eventOk)
		addSignal("sigStoryFinished")
	end
	def setFlow(flow)
		@flow=flow
		updateText
	end
	def setText(text)
		toAGEdit(getChild("text")).setText(text)
	end
	def setTitle(text)
		# it's AGText # FIXME: maybe exchange all AGText usages by AGEdit
		toAGText(getChild("title")).setText(text)
	end

	def updateText
		c=@flow.get
		if c
			setTitle(c[0])
			setText(c[1])
			return true
		else
			return false
		end
	end
	
	# signals	
	def eventOk(e)
		if not updateText
			sigStoryFinished(e)
			puts "pCaller:"+e.getCaller.getName
			getMap().unpause()
			toAGWindow(getChild("window")).close
		end
	end
end

class AntInspectWindow<AntDialog
	def initialize(p,text)
		super(p,"data/gui/layout/inspect.xml")
		getChild("text").setText(text)
	end
end

class AntQuitDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/quitquery.xml")
		setName("QuitDialog")
	end
	def eventOk(e)
		$app.tryQuit
	end
end

class AntOptionsDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/optionsmenu.xml")
		setName("OptionsDialog")
		addHandler(getChild("editmode"),:sigClick,:eventEditmode)
		addHandler(getChild("save"),:sigClick,:eventSave)
		addHandler(getChild("load"),:sigClick,:eventLoad)
		getMap.pause
	end
	def eventEditmode
		$app.enableEdit
	end
	def eventSave
		$app.save
		hide
	end
	def eventLoad
		$app.load
		hide
	end
	def eventOk(e)
		getMap.unpause
		super
	end
end

class AntSaveDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/savedialog.xml")
	end
	def eventOk(e)
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
	def eventOk(e)
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
	def eventOk(e)
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
		
		getChild("NpcTypeView").hide if @ent.class!=AntNPC
		if @ent.class==AntNPC
			@npcTypeW=toAGEdit(getChild("NpcType"))
			@npcTypeW.setText(@ent.npcType)
		end
	end
	def eventOk(e)
		menCountW=toAGEdit(getChild("MenCount"))
		nameW=toAGEdit(getChild("AntName"))
		@ent.setName(nameW.getText)
		@ent.setXMLProp("men",menCountW.getText)
		hide
		@ent.npcType=@npcTypeW.getText if @ent.class==AntNPC
		@ent.setupMesh
	end
end