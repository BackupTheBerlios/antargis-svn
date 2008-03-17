
require 'ruby/spec/level_testing.rb'


class Store
	def initialize
		@store={}
	end
	def [](name)
		@store[name]
	end
	def []=(name,value)
		@store[name]=value
	end
end

describe "HL-MoveJob" do
	it "should format correctly"
	it "should use only limited count of ll-jobs"
end

describe "Recruit job - run through" do
  include LevelTesting
  before(:all) do
    getTestApp(:tutorial0)
	@store=Store.new
  end
  it "should start off with hero with no men" do
  	advance
  	clickAwayStory
  	hero("Rowen").getMen.length.should == 1
  end
  it "starting recruit-job should work" do
  	rowen=hero("Rowen")
	rowen.newHLRecruitJob(building("Keep"))
	rowen.getJob.should be_a_kind_of(AntHeroRecruitJob)
  end

  it "should then send hero to tower (at most 10 low-level move-jobs for format and move)" do
    rowen=hero("Rowen")
    keep=building("Keep")

  	# format
    while rowen.getJob.stateName==:moveComplete
	  advance
    end
	
		rowen.getJobName.should =="moveJob"
		rowen.getTarget.should be_a_kind_of(AntMan)
		@store[:oldPos]=rowen.getPos2D
  end
  it "hero should only go fetch one man;go back and then stay at his position" do
		rowen=hero("Rowen")
		man=rowen.getTarget
		runUntilLowLevelJobToFinish(rowen)
		rowen.getJobName.should == "moveJob"
		rowen.getTarget.should be_nil
		trials=0
	while rowen.getJobName=="moveJob"
        runUntilLowLevelJobToFinish(rowen)
		trials+=1
		raise "too many trials" if trials>10
	end
	(rowen.getPos2D-@store[:oldPos]).length.should < 0.1
  end
  it "all men should stand at the sitting-position when job is finished" do
  	while rowen.getJob.stateName == :recruit
  		advance
	end
 
	
	clickAwayStory
	rowen.getJob.should_not be_a_kind_of(AntHeroRecruitJob)
	rowen.formation.should be_a_kind_of(AntFormationRest)
	rowen.getMen.each{|man|
		(man.getPos2D-rowen.getFormation(man,rowen.getPos2D)).length.should < 0.1
	}
  end
  
  def rowen
  	hero("Rowen")
  end
end


describe "Recruit job - aggression" do
  include LevelTesting
	it "men should fetch a few men if aggression is low"
  it "men should fetch all men if aggression if high"
end


describe "Build job" do
	include LevelTesting
	before(:all) do
	  getTestApp(:tutorial3)
	  @store=Store.new
	end
    it "should run at all" do
    	waitForStory
		clickAwayStory
    	rowen=hero("Rowen")
		dist=AGVector2.new(-5,8)
		rowen.newHLBuildJob(rowen.getPos2D+dist,AntTower)
		map.getBuildings.length.should ==0
		steps=0
		while rowen.getJob and rowen.getJob.is_a?(AntHeroBuildJob)
			advance
			steps.should < 100
		end
		map.getBuildings.length.should == 1
		map.getBuildings[0].should be_a_kind_of(AntTower)
    end
	it "- the tower should be of Rowen" do
		hero("Rowen").getPlayer.should equal(map.getBuildings[0].getPlayer)
	end
	it "should not work on water" do
		getTestApp(:tutorial3)
		rowen=hero("Rowen")
		waitForStory
		clickAwayStory
    widget("MessageBox").should be_nil
		rowen.newHLBuildJob(rowen.getPos2D+AGVector2.new(-5,-10),AntTower)
    widget("MessageBox").should_not be_nil
    widget("MessageBox").should be_visible
    widget("MessageBox").child("text").getText.should ==_("You can't build on water.")
		rowen.getJob.should_not be_a_kind_of(AntHeroBuildJob)
	end
  it "should use meshes correctly (in correct order)"
  it "should find resources correctly"
  
	describe "Continue a build job" do
		it "should work correctly"
	end
  
  it "should let people rest enough when aggression is low"
	it "should let people rest at the correct position"
end


