#!/usr/bin/env ruby

#require 'ruby/antargislib.rb'

def getCampaignFiles
	Dir[File.join("data","campaigns","*.xml")]
end

def getCampaigns
	$campaigns||=getCampaignFiles.collect{|f|Campaign.new(f)}
end

class CampaignLevel
	def initialize(node)
		
	end
end

class CutSceneDisplay<AGApplication
	#include AGHandler
	def initialize(image,text)
		super()
		@layout=AGLayout.new(nil,loadFile("data/gui/layout/cutscene.xml"))
		setMainWidget(@layout)
		addHandler(@layout.getChild("continue"),:sigClick,:sigQuit)
		@text=@ctext=""
		@time=0
		@speed=0.1
	end
	def sigQuit
		tryQuit
	end
	def eventKeyDown(e)
		super
		tryQuit
	end
	
	def setImage(image)
		w=@layout.getChild("image")
		if image
			w.show
			w.setSurface(image)
		else
			w.hide
		end
	end
	
	def setText(text)
		@text=text
		@ctext=""
		setTextForReal("")
	end
	
	def eventFrame(t)
		@time+=t
		old=@ctext
		while @time>@speed
			len=[@ctext.length+1,@text.length].min
			@ctext=@text[0..(len-1)]
			if old==@ctext
				break
			end
			if @ctext[-1..-1]!=" "
				@time-=@speed
			end
		end
		if old!=@ctext
			setTextForReal(@ctext)
		end
	end
	
	private
	def setTextForReal(text)
		w=@layout.getChild("text")
		if text
			w.show
			w.setText(text)
		else
			w.hide
		end
	end
end

class CutScene
	def initialize(node)
		@screens=[]
		puts node.getName
		node.getChildren("screen").each{|s|
			screen={}
			screen[:text]=""
			s.getChildren("image").each{|c|
				imageName=c.get("filename")
				if imageName
					screen[:image]=getSurfaceManager.loadSurface(imageName)
				end
			}
			s.getChildren("text").each{|c|
				screen[:text]+=c.get("text")
			}
			@screens.push(screen)
		}
	end
	def play
		display=CutSceneDisplay.new(@image,@text)
		@screens.each{|s|
			display.setImage(s[:image])
			display.setText(s[:text])
			display.run
		}
		GC.start
	end
end

class Campaign
	attr_reader :name, :image, :imageName, :description, :texture
	def initialize(filename)
		@doc=Document.new(filename)
		@xmlRoot=@doc.root
		
		@name=@xmlRoot.get("name")
		@imageName=@xmlRoot.get("image")
		@image=getSurfaceManager.loadSurface(@imageName)
		@texture=getTextureManager.makeTexture(@image)
		@description=@xmlRoot.get("description")
		@partID=0
		
		@part=[]
		@xmlRoot.getChildren.each{|c|
			case c.getName
				when "cutscene"
					@part.push(CutScene.new(c))
				when "level"
					@part.push(CampaignLevel.new(c))
			end
		}
	end
	
	def getCurrentPart
		@part[@partID]
	end
	
end

#getCampaigns.each{|c|puts c.name+" "+c.imageName}

def startCampaign(campaign)
	part=campaign.getCurrentPart
	part.play
end