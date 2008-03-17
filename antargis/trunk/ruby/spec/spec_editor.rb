
require 'ruby/editor_app.rb'
require 'ruby/gui/testing.rb'
require 'ruby/spec/level_testing.rb'
require 'ostruct'

describe "Editor" do
  it "should start up correctly" do
    @app=makeTestAppClass(AntEditorApp).new(1024,768)
    @mainWidget=@app.getMainWidget
    @app.getMap.getTerrainMesh.should_not be_nil
  end
end

describe "Editor" do
  #include LevelTesting
  include GuiTest
  before(:each) do
    @app=makeTestAppClass(AntEditorApp).new(1024,768)
    @mainWidget=@app.getMainWidget
    @app.getMap.initTerrainMesh
    @app.step    
  end
  it "should be possible to load a savegame" do
    @app.getMap.loadMap("data/levels/savegame0.antlvl").should be_true
    @app.getMap.getAllEntities.length.should > 0
  end
  it "should place entities at the correct position"
  it "should edit the map at the correct position"
  
  describe "AntEditPropDialog" do
    it "should be possible to edit a hero" do
      add(AntHero,AGVector2.new(10,10))
      hero=entities[0]
      edit(hero)
      window=widget("OptionsMenu")
      window.child("AntName").setText(AGStringUtf8.new("Bantor"))
      window.child("MenCount").setText(AGStringUtf8.new("10"))
      window.child("ok").click
      hero.getName.should=="Bantor"
      hero.getXMLProp("men").should=="10"
    end
  end
  
  describe "placing entities" do
    it "should place entities at exactly specified position" do
      targetPos=AGVector2.new(10.5,10.7)
      add(AntHero,targetPos)
      @app.getMap.getAllEntities.length.should == 1
      (@app.getMap.getAllEntities[0].getPos2D-targetPos).length.should < 0.1
    end
    it "should place entitiy at exact screen position" do
      focusMap(AGVector2.new(30,30))
      screenPos=AGVector2.new(200,200)
      addOnScreen(AntHero,screenPos)
      hero=@app.getMap.getAllEntities[0]
      pos=@app.getScene.getPosition(AGVector4.new(hero.getPos3D,1))
      (pos-screenPos).length.should < 5
    end
  end
  
  describe "modifying height" do
    it "should modify the map around the specified position"
    it "should use the front part of a terrain if occluded" 
    #do
#      p=AGVector2.new(33.7,32.3)
#      focusMap(p)
#      clickScreenMiddle
#      @app.step
#      clickScreenMiddle
#      @app.step
#      clickScreenMiddle
#      @app.step
#      clickScreenMiddle
#      run
#    end
  end
  
  describe "editing players" do
    it "should be possible to create a player" do
      widget("playerButton").click
      dialog=widget("PlayersDialog")
      dialog.child("playerName").setText(AGStringUtf8.new("Bantor"))
      dialog.child("addPlayer").click
    end
    it "should be possible to change a player's AI"
  end
  
  private
  def entities
    @app.getMap.getAllEntities
  end
  def edit(entity)
    focusMap(entity.getPos2D)
    selectTool
    clickScreenMiddle
  end
  def clickScreenMiddle
    clickScreen(getScreen.getWidth/2,getScreen.getHeight/2)
  end
  def selectTool
    widget("pointer").click
  end
  def add(entType,at)
    enableWidgetTab
    selectEntityButton(entType)
    clickMap(at)
  end
  def addOnScreen(entType,at)
    entCount=@app.getMap.getAllEntities.length
    enableWidgetTab
    selectEntityButton(entType)
    clickScreen(at.x,at.y)
    @app.getMap.getAllEntities.length.should == entCount+1
  end
  
  def enableWidgetTab
    widget("entities").click
  end
  def selectEntityButton(type)
    widget("entitiesTab").child(type.to_s).click
  end
  def clickMap(pos2d,button=1)
    @app.step
    @app.getMap.initTerrainMesh
    @app.getMap.getTerrainMesh.should_not be_nil
    list=[Struct.new(:node,:pos).new(@app.getMap.getTerrainMesh,pos2d)]
    lambda {@app.eventClick(list,button)}.should cross(@app,:addEntity)
    @app.eventClick(list,button)
    @app.getMap.getAllEntities.length.should == 1
  end
  
  def focusMap(at)
    @app.setCamera(at)
  end
  
  def run
    while true; @app.step;end
  end
  
  
end