require 'ruby/spec_helper.rb'
require 'ruby/campaign.rb'

describe Campaign do
	before(:all) do
		@campaigns=getCampaigns
		@tutorial=@campaigns[0]
 		@birth=@campaigns[1]
	end
	it "getCampaigns should return at least 2" do
		@campaigns.length.should >=2
 	end
	it "first campaign is tutorial" do
		@tutorial.name.should == "Tutorial"
	end

	it "tutorial starts with level" do
		@tutorial.getCurrentPart.should be_a_kind_of(CampaignLevel)
	end

 	it "birth campaign is second campaign" do
 		#pp @birth.name
 		@birth.name.should == "A hero is born."
 	end

	it "birth campaign starts with cutscene" do
		@birth.getCurrentPart.should be_a_kind_of(CutScene)
	end
 	it "should run a story fine" do
		OldDisplay=CutSceneDisplay.clone
		class CutSceneDisplay
			@@ok=false
			@@calls={}
			
			# remove all methods
			self.instance_methods.each{|m|undef_method(m) unless m=~/__.*__/}
			def initialize
				@@ok=true
			end
			def CutSceneDisplay.initok
				@@ok 
			end
			def CutSceneDisplay.callCount(name)
				@@calls[name]
			end
			def method_missing(name,*s)
				@@calls[name]||=0
				@@calls[name]+=1
			end
		end
 		cutscene=@birth.getCurrentPart
 		lambda {cutscene.play}.should cross(CutSceneDisplay,:initialize)
		CutSceneDisplay.initok.should be_false # because it wasn't hit due to "should cross"
		CutSceneDisplay.callCount(:setImage).should >0
		CutSceneDisplay.callCount(:setImage).should equal(CutSceneDisplay.callCount(:setText))
		CutSceneDisplay.callCount(:setImage).should equal(CutSceneDisplay.callCount(:run))
 		CutSceneDisplay=OldDisplay
 	end


end
