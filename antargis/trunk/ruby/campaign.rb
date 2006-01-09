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

class CutScene
	def initialize(node)
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
	
end

#getCampaigns.each{|c|puts c.name+" "+c.imageName}