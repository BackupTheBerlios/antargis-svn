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


# draws a simple rect all of the defined region (typically the whole screen)
# this used for darkening the screen when the game is paused.
#
# the default time for blending to dark is 500 ms (this is not changeable)
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
		# eat up the events when visible
		if visible
			return true
		else
			return super(e)
		end
	end
end

class AntDialog<AGLayout
	include AGHandler
	def initialize(parent,filename,fade=true)
		super(parent)
		loadXML(loadFile(filename)) # it's safe here to call loadXML in constructor as "self" is already a ruby-object
		addHandler(getChild("ok"),:sigClick,:eventOk)
		if getChild("cancel")
			addHandler(getChild("cancel"),:sigClick,:eventCancel)
		end
		window=getChild("window")
		addHandler(window,:sigClose,:eventClose)
		addSignal("sigClosed")
		#setModal(true)
		if fade
			addChildBack(RectWidget.new(self,getRect.origin,0.5))
			setQuietVolumeWave
		end
	end
	def eventOk(e)
		eventClose(e)
		return true
	end
	def eventCancel(e)
		eventClose(e)
		return true
	end
	
	def eventKeyDown(e)
		if super then return true end
		case e.getKey
			when SDLK_ESCAPE
				eventClose(e)
				getMap.pause=false
				return true
			when SDLK_RETURN, SDLK_SPACE
				eventOk(e)
		end
		return false
	end
	def eventClose(e)
		close
		sigClosed(AGEvent.new(self,"sigClosed"))
		setNormalVolumeWave
		return true
	end
end

class AntStoryTalk<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/storytalk.xml",false)
		getMap().pause=true
		addHandler(getChild("window"),:sigClose,:eventOk)
		addHandler(getChild("back"),:sigClick,:eventBack)
		addHandler(getChild("forward"),:sigClick,:eventClose)
		addSignal("sigStoryFinished")
	end
	def setFlow(flow)
		if @flow.nil?
			@flow=flow
		else
			@flow.append(flow)
		end
		updateText
	end
	def setText(text)
		toAGEdit(getChild("text")).setText(_(text))
	end
	def setTitle(text)
		# it's AGText # FIXME: maybe exchange all AGText usages by AGEdit
		toAGText(getChild("title")).setText(_(text))
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
	def eventClose(e)
		if not updateText
			sigStoryFinished(e)
			getMap().pause=false
			hide
			super
		else
			getChild("window").show
		end
		return true
	end
	def eventBack(e)
		@flow.back
		@flow.back
		updateText
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
			getChild("window").getChild("title").setText(_(@ent.getName))
		end
	end
end

class AntQuitDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/quitquery.xml")
		setName("QuitDialog")
		getMap.pause=true
	end
	def eventClose(e)
		super
		getMap.pause=false
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
		addHandler(getChild("story"),:sigClick,:eventStory)
		addHandler(getChild("video"),:sigClick,:eventVideo)
		addHandler(getChild("audio"),:sigClick,:eventAudio)
		getMap.pause=true
	end
	def eventStory
		close
		f=StoryFlow.new("")
		$app.tellStory(f)
		return true
	end
	def eventVideo
		$app.videoOptions
		close
		return true
	end
	def eventAudio
		$app.audioOptions
		close
		return true
	end
	def eventSave
		$app.save
		close
		return true
	end
	def eventLoad
		$app.load
		close
		return true
	end
	def eventClose(e)
		super
		getMap.pause=false
		return true
	end
end

def takeScreenshot
	$app.hidePanel
	$app.draw
	s=getScreen.screenshot
	$app.showPanel
	s
end

def takeSmallScreenshot
	takeScreenshot.scale(256,192)
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
		hide
		getMap.saveMap("savegames/"+filename)
		takeSmallScreenshot.save("savegames/#{filename.gsub("antlvl","png")}")
		getMap.pause=false
		setNormalVolumeWave
		close
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
		hide
		$campaign.save("savegames/"+filename)
		takeSmallScreenshot.save("savegames/#{filename.gsub("antcmp","png")}")
		#getMap.saveMap("savegames/"+filename)
		getMap.pause=false
		close
		return true
	end
	def eventClose(e)
		super
		getMap.pause=false
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
		getMap.pause=false
		close
		return true
	end
	def eventClose(e)
		super
		getMap.pause=false
		return true
	end
end


# AntAudioOptionsDialog is the audio-dialog in-game
# you can set music,ambient and sound volumes there
# volumes are stored into the config.xml file.
class AntAudioOptionsDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/dialog_audio_options.xml")

		@barnames=["music","ambient","sound"]
		@bars={}
		@barnames.each{|n|
			@bars[n]=getChild(n+"Bar")
		}
		@barnames.each{|n|
			cname="inc#{n[0..0].upcase+n[1..-1]}"
			addHandler(getChild(cname),:sigClick,:eventModBar)
			cname="dec#{n[0..0].upcase+n[1..-1]}"
			addHandler(getChild(cname),:sigClick,:eventModBar)
		}

		getMap.pause=true
		readVolumes
	end

	# eventHandler for a + or - Button being pressed
	# resets set bar-values then.
	def eventModBar(e)
		name=e.getCaller.getName
		what=name[0..2]
		barname=name[3..-1].downcase
		case what
			when "inc"
				@bars[barname].setValue(@bars[barname].getValue+0.1)
			when "dec"
				@bars[barname].setValue(@bars[barname].getValue-0.1)
		end
		updateVolumesP
	end
	def eventOk(e)
		getMap.pause=false
		close
		return true
	end
private
	# read volumes from config-file
	def readVolumes
		@barnames.each{|n|
			@bars[n].setValue(getConfig.get("#{n}Volume").to_f)
		}
	
		updateVolumesP
	end
	# saves volumes to config-file and sets current volumes
	def updateVolumesP
		@bars.each{|n,v|
			getConfig.set("#{n}Volume",v.getValue.to_s)
		}
		updateVolumes
	end
end


class AntVideoOptionsDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/dialog_video_options.xml")
		addHandler(getChild("fullscreen"),:sigClick,:eventFullscreen)
		addHandler(getChild("shadow"),:sigClick,:eventShadow)
		addHandler(getChild("animation"),:sigClick,:eventAnimation)
		addHandler(getChild("particle"),:sigClick,:eventParticle)
		addHandler(getChild("1024"),:sigClick,:event1024)
		addHandler(getChild("1280"),:sigClick,:event1280)
		addHandler(getChild("1400"),:sigClick,:event1400)
		getMap.pause=true
	end
	def eventOk(e)
		getMap.pause=false
		close
		return true
	end
	def eventFullscreen
	  getMain.toggleFull
		return true
	end
	def eventParticle
		toggleConfig("particleEffects")
	end
	def eventAnimation
		toggleConfig("animationType")
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
		getMain.initVideo(w,h,32,getMain.fullscreen,true)
		return true
	end
private
	def toggleConfig(n)
		if getConfig.get(n)=="false"
			getConfig.set(n,"true")
		else
			getConfig.set(n,"false")
		end
	end
end

class AntPauseDialog<AntDialog
	def initialize(parent)
		super(parent,"data/gui/layout/pause.xml")
		setName("PauseDialog")
		if getMap
			getMap.pause=true
		end
	end
	def eventOk(e)
		super
		getMap.pause=false
		#hide
		return true
	end
end

class AntQueryDialog<AntDialog
	def initialize(parent,file=nil,&block)
		if file==nil
			file="data/gui/layout/query_dialog.xml"
		end
		super(parent,file)

		@block=block

		setName("PauseDialog")
		if getMap
			getMap.pause=true
		end
	end
	def eventCancel(e)
		super
		getMap.pause=false
		return true
	end

	def eventOk(e)
		super
		getMap.pause=false

		@block.call
		return true
	end
end

# draw a simple (loading) bar
class AGBar<AGWidget
	# contructor-parameters:
	# * p - parent widget
	# * r - rectangle of widget in client area of p
	# * c - color of the bar
	# * bc - background-color of the bar
	# the default value is 0.5
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
	# get the current value
	def getValue
		@value
	end
	# sets the bar to the value of v, which should be between 0 and 1
	def setValue(v)
		@value=[0,v,1].sort[1]
	end

	# draw 2 super-imposed gradients with painter p
	# this is called automatically
	def draw(p)
		p.drawGradient(getRect.origin,@b0,@b1,@b2,@b3)
		p.drawGradient(AGRect.new(0,0,width*@value,height),@c0,@c1,@c2,@c3)
	end

	# load value from XML - parameter "value"
	def loadXML(n)
		super
		@value=n.get("value").to_f
		redraw
	end
end


# the creator class for a AGBar
class AGBarCreator<AGLayoutCreator
	def initialize
		super("bar")
	end
	# overrides the AGLayoutCreator function
	# * p Widget parent
	# * r client rectangle
	# * n XML-Node describing the agbar
	def create(p,r,n)
		c=AGColor.new(n.get("color"))
		bc=AGColor.new(n.get("bgcolor"))
		w=AGBar.new(p,r,c,bc)
		return w
	end
end
$agBarCreator=AGBarCreator.new  # register creator class


# a widget for displaying the "loading..." screen
class LoadScreen<AGLayout
	def initialize(p)
		super(p)
		loadXML(loadFile("data/gui/layout/loadscreen.xml")) # ok - see at AntDialog
	end
end

# this is an application object for showing the screen with loading... bar
# it's typical use goes like this:
# loadscreen=LoadApp.new
# loadscreen.setvalue(somevalue)
# loadscreen.tick
# -- do some stuff here --
# loadscreen.setvalue(somevalue)
# loadscreen.tick
# and so on
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

require 'ant_messagebox.rb'

