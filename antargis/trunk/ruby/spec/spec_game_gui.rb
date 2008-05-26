
require 'ruby/spec/level_testing.rb'

if false

describe "Aggression buttons" do
  include LevelTesting
  before(:all) do
    getTestApp(:tutorial0)
  end
  it "should not call pp" do
    lambda {widget("doAgg0").click}.should_not cross(:pp)
  end
end

describe "Gui-Selecting" do
    include LevelTesting
    before(:all) do
      getTestApp(:tutorial0)
      waitForStory
      clickAwayStory
    end
  
  it "should be possible to select tower" do
    tower=entities(AntTower)[0]
    lambda{tower.click}.should change(tower,:selected)
  end
  
end

describe "Building with gui interaction" do
    include LevelTesting
    before(:all) do
      getTestApp(:tutorial3)
    waitForStory
    clickAwayStory
    end
  it "should start at all" do
    widget("doBuild").click
    advance
    clickMap(hero("Rowen").getPos2D+AGVector2.new(-5,10))
        widget("AntTower").should be_a_kind_of(AGButton)
        widget("AntTower").click
    hero("Rowen").getJob.should be_a_kind_of(AntHeroBuildJob)
    advance(15)
    map.getByType(AntBuildingSite).should have(1).item
  end
  it "should be possible to stop and rest" do
    widget("doRest").click
        hero("Rowen").getJob.should_not be_a_kind_of(AntHeroBuildJob)
        hero("Rowen").getJob.should be_a_kind_of(AntHeroRestJob)
  end
  it "should be possible to continue building a tower" do
    site=entities(AntBuildingSite)[0]
    lambda{site.click}.should change(site,:selected)
    widget("doContinue").click
    hero("Rowen").getJob.should be_a_kind_of(AntHeroBuildJob)
    advance(100)
        hero("Rowen").getJob.should be_a_kind_of(AntHeroRestJob)
    entities(AntBuildingSite).should have(0).items
    entities(AntTower).should have(1).item
    entities(AntTower)[0].getPlayer.should == hero("Rowen").getPlayer

  end
  
  it "toggling fullscreen should'nt destroy anim-meshes"
end

describe AntNameDisplay do
  it "should display a red text for enemies"
  it "should display a yellow text for friends"
  it "should switch text color when enemy is pwned"
end

describe AntInfoBox do
    include LevelTesting
    before(:all) do
      getTestApp(:tutorial0)
      waitForStory
      clickAwayStory
    end

  it "should be displayed when hero is badly hurt" do
     widget("InfoBox").should be_nil#
     hero("Rowen").decEnergy(0.9)
     advance
       widget("InfoBox").should_not be_nil
       widget("InfoBox").should be_visible
  end
end

end

describe "Minimap" do
  include LevelTesting
  before(:all) do
    getTestApp(:tutorial0)
    waitForStory
    clickAwayStory
  end
  
  it "should be connected to the mainview" do
    curpos=@app.getScene.getCamera
    curpos.should be_a_kind_of(AGVector4)
    
    newpos=@app.getScene.getCamera
    newpos.should == curpos
    
    minimap=widget("miniMap")
    minimap.should_not be_nil
    #@app.step while true
    click(minimap.getScreenRect.getMiddle+AGVector2.new(10,10))
    newpos=@app.getScene.getCamera
    newpos.should be_a_kind_of(AGVector4)
    newpos.should_not == curpos
  end
end

