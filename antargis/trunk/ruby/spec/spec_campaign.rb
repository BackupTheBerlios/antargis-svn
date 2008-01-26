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
		cutscene=@birth.getCurrentPart
		lambda {cutscene.play}.should_not cross(CutSceneDisplay,:new)
		
	end


end
