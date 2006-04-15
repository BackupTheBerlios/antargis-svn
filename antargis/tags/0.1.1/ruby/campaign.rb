#!/usr/bin/env ruby

#require 'ruby/antargislib.rb'

def getCampaignFiles
	Dir[File.join("data","campaigns","*.xml")]
end

def getCampaigns
	$campaigns||=getCampaignFiles.collect{|f|Campaign.new(f)}.select{|f|f.enabled}
end

class CampaignLevel
	attr_reader :result
	def initialize(node)
		@level=node.get("file")
		node.getChildren("failscene").each{|c|
			@lostScene=CutScene.new(c)
		}
		@current=node.get("current")
		if @current==""
			@current=@level
		end
	end
	def save(n)
		n.set("file",@level)
		if @app
			raise 'Problem - $campaign not net' if $campaign==nil
			sname=$campaign.savename.gsub("antcmp","antlvl")
			n.set("current",sname)
			getMap.saveMap(sname)
		end
	end
	def play
		loadscreen=LoadApp.new
		loadscreen.setValue(0)
		loadscreen.tick
		begin
			@app=AntGameApp.new(@current,getMain.width,getMain.height,loadscreen)
			@app.run
			@result=@app.result
			@finish=@app.finished
			@app=nil
			getSoundManager.stopAllChannels
		end
		$app=nil
		$map=nil
		$antView=nil
		$buttonPanel=nil
		$inventory=nil

		$screen=nil
		GC.start
		
		case @result.won
			when "lost"
				@lostScene.play
		end

	end

	def result
		@result
	end

	def finished
		@finish
	end
end

class CutSceneDisplay<AGApplication
	#include AGHandler
	def initialize(image,text)
		super()
		@layout=AGLayout.new(nil)
		@layout.loadXML(loadFile("data/gui/layout/cutscene.xml"))
		setMainWidget(@layout)
		addHandler(@layout.getChild("continue"),:sigClick,:eventQuit)
		@text=@ctext=""
		@time=0
		@speed=0.1
	end
	def eventQuit
		if @finished
			tryQuit
		else
			@ctext=@text
			setTextForReal(@ctext)
			@finished=true
			#eventFrame(10000) # as if much time has passed
		end
		return true
	end
	def eventKeyDown(e)
		eventQuit
# 		if @text==@ctext
# 			tryQuit
# 		else
# 			@ctext=@text
# 			setTextForReal(@ctext)
# 		end
		return super
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
			if @ctext==@text
				@finished=true
			end
		end
		return true
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
		node.getChildren("screen").each{|s|
			screen={}
			screen[:text]=""
			s.getChildren("image").each{|c|
				imageName=c.get("filename")
				screen[:imageName]=imageName
				if imageName
					screen[:image]=AGSurface.load(imageName)
				end
			}
			s.getChildren("text").each{|c|
				screen[:text]+=c.get("text")
			}
			@screens.push(screen)
		}
		@finished="won"
	end
	def save(n)
		@screens.each{|s|
			c=n.addChild("screen")
			c.addChild("text").set("text",s[:text]) if s[:text]
			c.addChild("image").set("filename",s[:imageName]) if s[:imageName]
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
	def result
		GameResult.new("won")
	end
end

class Campaign
	attr_reader :name, :image, :imageName, :description, :texture, :enabled
	def initialize(filename)
		puts "NEW CAMPAIGN FOMR FILE:",filename
		if not fileExists(filename)
			raise "file not found #{filename}"
		end
		puts "CONT"
		@doc=Document.new(filename)
		puts "CONT"
		@xmlRoot=@doc.root
		@enabled=(@xmlRoot.get("enabled")!="false")
		@name=@xmlRoot.get("name")
		@imageName=@xmlRoot.get("image")
		puts "imageName:",@imageName
		@image=AGSurface.load(@imageName)
		@texture=AGTexture.new(@image)
		@description=@xmlRoot.get("description")
		@partID=@xmlRoot.get("part").to_i
		
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
	def savename
		@savename
	end
	def save(filename)
		@savename=filename
		doc=Document.new
		root=doc.root
		root.setName("campaign")
		root.set("name",@name)
		root.set("image",@imageName)
		root.set("description",@description)
		root.set("part",@partID.to_s)
		@part.each{|p|
			c=nil
			case p.class.to_s
				when "CutScene"
					c=root.addChild("cutscene")
				when "CampaignLevel"
					c=root.addChild("level")
				else
					raise "Unknown class type:"+p.class.to_s
			end
			p.save(c)
		}
		
		saveFile(filename,doc.toString)
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

def startCampaign(campaign)
	campaign=campaign.dup # copy
	continueCampaign(campaign)
end

def continueCampaign(campaign)
	begin
		$campaign=campaign
		part=campaign.getCurrentPart
		part.play
		if part.result.won=="won"
			if campaign.proceed
				break
			end
		end
	end while part.result.won!="canceled"
	campaign.save("campaign0.antcmp") # FIXME:test save
	$campaign=nil
end