require 'ruby/gui/testing.rb'

class DragWidget<AGWidget
  def initialize(p,r)
    super
  end
  def draw(p)
    p.fillRect(getRect.origin,AGColor.new(0xFF,0xFF,0))
  end
  def eventDragBy(e,d)
    r=super
    setRect(getRect+d)
    r
  end
end

class TestApp<AGApplication
  attr_reader :m, :b
  attr_accessor :l
  
  def initialize
    super
    @l=[]
    @m=AGWidget.new(nil,AGRect2.new(10,10,100,100))
    @b=AGButton.new(@m,AGRect2.new(10,10,40,40))
    @d=DragWidget.new(@m,AGRect2.new(70,70,10,10))
    @m.addChild(@b)
    @m.addChild(@d)
    setMainWidget(@m)
    [@b,@m].each{|w|
      addHandler(w,:sigClick,:clicked)
    }
  end
  def eventFrame(t)
    super
    delay(100)
  end
  def clicked(e)
    @l<<e.getCaller
  end
end



describe "MouseEvents on simple widgets (no scaling or hiding)" do
  include GuiTest
  before(:all) do
    @app=makeTestAppClass(TestApp).new
  end
  it "should set hover to true on button when hovering" do
    mouseMotion(AGVector2.new(1,1))
    @app.m.hovered.should == false
    @app.b.hovered.should == false
    mouseMotion(AGVector2.new(11,11))
    @app.m.hovered.should == true
    @app.b.hovered.should == false
    mouseMotion(@app.b.getScreenRect.getMiddle)
    @app.b.hovered.should == true
  end
  
  it "should handle clicking with correct positions" do
    @app.l.should == []
    click(AGVector2.new(1,1))
    @app.l.should == []
    click(AGVector2.new(11,11))
    @app.l.should == [@app.m]
    click(AGVector2.new(21,21))
    @app.l.should == [@app.m,@app.b]
    
  end
  
  it "shuold handle dragging correctly" do
    @app.step while true
  end
end

describe "MouseEvents on complex widgets (with scaling and hiding)" do
  it "should handle hovering correctly"
  it "should handle clicking correctly"
end
