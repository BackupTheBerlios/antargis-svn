require File.join(File.split(__FILE__)[0],"..","..","spec_helper.rb")
require File.expand_path("../../../gui/testing.rb",__FILE__)
require File.expand_path("../app.rb",__FILE__)

describe "Campaign editor" do
  include GuiTest
  before(:all) do
    @app=makeTestAppClass(CampaignEditorApp).new
  end
  it "should be possible to place levels on the grid" do
    drag(getSourceMiddle("levelSource"),getGridPos(1,1),10)
    grid.getAllDescendants.select{|c|c.is_a?(DragBox)}.length.should == 1
    level=grid.getChildren[0]
    level.should be_a_kind_of(DragBoxLevel)
    level.getParent.should_not be_nil
  end

  it "should be possible to move nodes and they should be visible at all times" do
    level=grid.getChildren[0]
    sr=level.getScreenRect
    dr=sr+AGVector2.new(100,0)

    drag(sr.getMiddle,dr.getMiddle,10) {|event|
      level.should be_visible
      level.should be_focused if event==:mouseMotion
    }
    drag(dr.getMiddle,sr.getMiddle,10)
  end

  #if false
  it "should be possible to move the grid" do
    level=grid.getChildren[0]
    level.getParent.should_not be_nil
    oldPos=level.getScreenRect.getMiddle
    delta=AGVector2.new(0,200)
    scrollWidget=widget("dragGrid").getParent
    ov=scrollWidget.getVector
    middle=widget("dragGrid").getScreenRect.getMiddle
    level.getParent.should_not be_nil
    drag(middle,middle-delta,5) 
    nv=scrollWidget.getVector
    newPos=level.getScreenRect.getMiddle
    level.getParent.should_not be_nil
    oldPos.should == newPos+delta 
    drag(middle-delta,middle,5)
  end
#end
  
  it "should not use widgets for DragTargets anymore" do
    # define DragTarget, if not yet defined
    class DragTarget
    end
    grid.getAllDescendants.select{|c|c.is_a?(DragTarget)}.length.should == 0
  end
  
  it "should be possible to select a node and edit the name" do
    grid=widget("dragGrid")
    somepos=AGVector2.new(800,600)
    mouseMotion(somepos)
    click(somepos)
    node=grid.getChildren[0]
    pos=node.getScreenRect.getMiddle
    observe(@app.getEffect("hideEdit"),:run) {
      observe(@app.getEffect("showEdit"),:run) {
        observe(grid.widget,:select) {
          mouseDown(pos)
          mouseUp(pos)
        }.should be_called
      }.should be_called
    }.should_not be_called
    # mouseUp(pos)
    observe(@app.getEffect("hideEdit"),:step) {
      observe(@app.getEffect("showEdit"),:step) {
        observe(@app,:eventFrame) {
          1.upto(20) do @app.step end
        }.should be_called
      }.should be_called(2,:times)
    }.should_not be_called
    widget("textEdit").should have_focus
    #@app.step while true
    widget("textEdit").height.should > 10
  end 
  
  it "should be possible to place stories on the grid"
  it "should be possible to define a start-node"
  it "should be possible to draw arrows from one node to another"
  it "should be possible to name grid-nodes"
  it "should be possible to name arrows"
  
  private
  def drag(from,to,frames,&block)
    p from,to
    #exit
    @app.setCursor(getTextureCache.get("blue_cursor.png"))
    mouseMotion(from)
    mouseDown(from)
    block.call(:mouseDown) if block
    1.upto(frames) {|f|
      mouseMotion(from+(to-from)*f/frames)
      @app.step
      block.call(:mouseMotion) if block
    }
    mouseUp(to)
    block.call(:mouseUp) if block
    #@app.step while true
  end

  
  def getSourceMiddle(name)
    getWidget(name).getScreenRect.getMiddle
  end
  
  def getWidget(name)
    @app.getMainWidget.getChild(name)
  end
  def getGridPos(x,y)
    gridWidth=50
    (AGRect2.new(x*gridWidth,y*gridWidth,gridWidth,gridWidth)+grid.getScreenRect.getV0).getMiddle
  end
  def grid
    getWidget("dragGrid")
  end
end

