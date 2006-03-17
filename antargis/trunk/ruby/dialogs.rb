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

class RectWidget<AGWidget
	def initialize(p,r,opac)
		super(p,r)
		@opac=opac
		@startTime=$app.getTicks
		@blendTime=500.0
	end
	def draw(p)
		t=$app.getTicks
		b=(t-@startTime)/@blendTime
		b=[1,b].min
		c=AGVector4.new(0,0,0,b*@opac)
		p.fillRect(getRect.origin,AGColor.new(c))
	end
	def eventMouseButtonDown(e)
		if visible
			return true
		else
			return super(e)
		end
	end
end

#
# Story talking window - appearance defined in storytalk.xml
#
class AntDialog<AGLayout
	include AGHandler
	def initialize(parent,filename,fade=true)
		super(parent,loadFile(filename))
		addHandler(getChild("ok"),:sigClick,:eventOk)
		if getChild("cancel")
			addHandler(getChild("cancel"),:sigClick,:eventCancel)
		end
		addSignal("sigClosed")
		#setModal(true)
		if fade
			addChildBack(RectWidget.new(self,getRect.origin,0.5))
			setQuietVolumeWave
		end
	end
	def eventOk(e)
		eventClose
		return true
	end
	def eventCancel
		eventClose
		return true
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
		return false
	end
	def eventClose
		hide
		sigClosed(AGEvent.new(self,"sigClosed"))
		setNormalVolumeWave
		return true
	end
end

class AntStoryTalk<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/storytalk.xml",false)
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

	def setFace(s)
		getChild("face").setSurface(s)
	end

	def updateText
		c=@flow.get
		if c
			setTitle(c[0])
			setFace(getMap.getPortrait(c[0]))
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
		return true
	end
end

class AntInspectWindow<AntDialog
	def initialize(p,ent)
		@ent=ent
		super(p,"data/gui/layout/inspect.xml")
	end
	def draw(p)
		update
		super
	end
private
	def update
		text=@ent.getDescription
		if @oldtext!=text
			getChild("text").setText(text)
			@oldtext=text
			getChild("window").getChild("title").setText(@ent.getName)
		end
	end
end

class AntQuitDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/quitquery.xml")
		setName("QuitDialog")
		getMap.pause
	end
	def eventClose
		super
		getMap.unpause
		return true
	end
	def eventOk(e)
		$app.tryQuit
		return true
	end
end

class AntOptionsDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/dialog_options.xml")
		setName("OptionsDialog")
		addHandler(getChild("save"),:sigClick,:eventSave)
		addHandler(getChild("load"),:sigClick,:eventLoad)
		addHandler(getChild("video"),:sigClick,:eventVideo)
		getMap.pause
	end
	def eventVideo
		$app.videoOptions
		hide
		return true
	end
	def eventSave
		$app.save
		hide
		return true
	end
	def eventLoad
		$app.load
		hide
		return true
	end
	def eventClose
		super
		getMap.unpause
		return true
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
		return true
	end
end

class AntSaveCampaignDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/savedialog.xml")
		if $campaign==nil
			raise "saving while not in campaign!"
		end
		getChild("window").getChild("title").setText("Save campaign")
	end
	def eventOk(e)
		filename=toAGEdit(getChild("Filename")).getText
		puts "FILENAME:"+filename
		if not filename =~ /.*\.antcmp/ then
			filename=filename+".antcmp"
		end
		$campaign.save("savegames/"+filename)
		#getMap.saveMap("savegames/"+filename)
		getMap.unpause
		hide
		return true
	end
	def eventClose
		super
		getMap.unpause
		return true
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
		return true
	end
	def eventClose
		super
		getMap.unpause
		return true
	end
end

class AntVideoOptionsDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/dialog_video_options.xml")
		addHandler(getChild("fullscreen"),:sigClick,:eventFullscreen)
		addHandler(getChild("shadow"),:sigClick,:eventShadow)
		addHandler(getChild("1024"),:sigClick,:event1024)
		addHandler(getChild("1280"),:sigClick,:event1280)
		addHandler(getChild("1400"),:sigClick,:event1400)
		getMap.pause
	end
	def eventOk(e)
		getMap.unpause
		hide
		return true
	end
	def eventFullscreen
	  getMain.toggleFull
		return true
	end
	def eventShadow
		s=$app.getScene
		case s.getShadow
			when 0
				s.setShadow(1)
			else
				s.setShadow(0)
		end
		return true
	end
	def event1024
		setRes(1024,768)
		return true
	end
	def event1280
		setRes(1280,1024)
		return true
	end
	def event1400
		setRes(1400,1050)
		return true
	end
	def setRes(w,h)
		getMain.changeRes(w,h,32,getMain.fullscreen,true)
		return true
	end
end

class AntPauseDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/pause.xml")
		setName("PauseDialog")
		if getMap
			getMap.pause
		end
	end
	def eventOk(e)
		super
		getMap.unpause
		#hide
		return true
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
		return true
	end
end

class AGBar<AGWidget
	def initialize(p,r,c,bc)
		super(p,r)
		@color=c
		@bcolor=bc
		@value=0.5

		@c0=c*0.7
		@c1=c*0.7
		@c2=c*1.2
		@c3=c

		@b0=bc*0.7
		@b1=bc*0.7
		@b2=bc*1.2
		@b3=bc
	end
	def setValue(v)
		@value=[0,v,1].sort[1]
	end
	def draw(p)
		p.drawGradient(getRect.origin,@b0,@b1,@b2,@b3)
		#p.fillRect(AGRect.new(0,0,width*@value,height),@color)
		p.drawGradient(AGRect.new(0,0,width*@value,height),@c0,@c1,@c2,@c3)

	end
end


class AGBarCreator<AGLayoutCreator
	def initialize
		super("bar")
	end
	def create(p,r,n)
		c=AGColor.new(n.get("color"))
		bc=AGColor.new(n.get("bgcolor"))
		w=AGBar.new(p,r,c,bc)
		return w
	end
end
$agBarCreator=AGBarCreator.new


class LoadScreen<AGLayout
	def initialize(p)
		super(p,loadFile("data/gui/layout/loadscreen.xml"))
	end
end

class LoadApp<AGApplication
	def initialize
		super
		@s=LoadScreen.new(nil)
		setMainWidget(@s)
	end
	def setValue(p)
		@s.getChild("bar").setValue(p)
	end
	def tick
		prepareDraw
		draw
	end
end