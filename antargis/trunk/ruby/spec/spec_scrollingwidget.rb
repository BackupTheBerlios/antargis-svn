require 'ruby/gui/testing.rb'

if false
class MyTestApp<AGApplication
  BIGRECT=AGRect2.new(0,0,200,200)
  SMALLRECT=AGRect2.new(0,0,100,100)
  BGCOLOR=AGColor.new(0,0,0)
  FGCOLOR=AGColor.new(0xFF,0,0)
  
  attr_reader :surface,:bgsurface
  
  def initialize
    super
    sw=AGScrollingWidget.new(nil,SMALLRECT)
    sw.setClientRect(BIGRECT)
    setMainWidget(sw)
    @surface=makeSurface(FGCOLOR)
    @bgsurface=makeSurface(BGCOLOR)
    image=AGImage.new(sw,SMALLRECT,@surface,false)
    sw.addChild(image)
    sw.addChild(AGButton.new(sw,sw.getRect))
  end
  
  private
  def makeSurface(color)
    w=SMALLRECT.width.to_i
    h=SMALLRECT.height.to_i
    s=AGSurface.new(w,h)
    0.upto(w-1){|x|
      0.upto(h-1){|y|
        s.putPixel(x,y,color)
      }
    }
    s
  end
end

describe "AGScrollingWidget" do
  before(:each) do
    @app=makeTestAppClass(MyTestApp).new
  end
  it "should draw the surface normally in init state" do
    @app.step
    @app.step
    screenshot=getScreen.screenshot(true)
    screenshot.save("muh.png")
    pp screenshot.getSubSurface(MyTestApp::SMALLRECT) == @app.surface
    screenshot.getSubSurface(MyTestApp::SMALLRECT).should == @app.surface
    @app.step while true
    
  end
  it "shouldn't draw anything when the widget is scrolled to the right lower corner"
end
end