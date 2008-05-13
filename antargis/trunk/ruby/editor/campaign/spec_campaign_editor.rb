
require File.join(File.split(__FILE__)[0],"..","..","spec_helper.rb")
require File.join(File.split(__FILE__)[0],"..","..","gui","testing.rb")
require File.join(File.split(__FILE__)[0],"drag_grid.rb")

describe "Campaign editor" do
  include GuiTest
  before(:all) do
    @app=makeTestAppClass(CampaignEditorApp).new
  end
  it "should be possible to place levels on the grid" do
    
    drag(getSourceMiddle("levelSource"),getGridPos(1,1),50)
  end
  it "should be possible to place stories on the grid"
  it "should be possible to define a start-node"
  it "should be possible to draw arrows from one node to another"
  it "should be possible to name grid-nodes"
  it "should be possible to name arrows"
  
  private
  def drag(from,to,frames)
    p from,to
    #exit
    @app.setCursor(getTextureCache.get("blue_cursor.png"))
    
    mouseDown(from)
    1.upto(frames) {|f|
      mouseMotion(from+(to-from)*f/frames)
      @app.step
    }
    mouseUp(to)
  end
  
  def getSourceMiddle(name)
    getWidget(name).getScreenRect.getMiddle
  end
  
  def getWidget(name)
    @app.getMainWidget.getChild(name)
  end
  def getGridPos(x,y)
    gridWidth=50
    (AGRect2.new(x*gridWidth,y*gridWidth,gridWidth,gridWidth)+getWidget("dragGrid").getScreenRect.getV0).getMiddle
  end
end