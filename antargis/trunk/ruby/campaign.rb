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
	def play
	end
	def finished
		true
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
		if @text==@ctext
			tryQuit
		else
			@ctext=@text
			setTextForReal(@ctext)
		end
	end
	
	def setImage(image)
		w=@layout.getChild("image")
		w2=@layout.getChild("imageFrame")
		if image
			w2.show
			w.setSurface(image)
		else
			w2.hide
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
	attr_reader :finished
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
		@finished=true
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
	def proceed
		@partID+=1
		return @partID>=@part.length
	end
	def restart
		@partID=0
	end
end

#getCampaigns.each{|c|puts c.name+" "+c.imageName}

def startCampaign(campaign)
	campaign.restart
	begin
		part=campaign.getCurrentPart
		part.play
		if campaign.proceed
			break
		end
	end until not part.finished
end