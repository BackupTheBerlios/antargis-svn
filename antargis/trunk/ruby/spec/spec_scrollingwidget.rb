require 'ruby/gui/testing.rb'

class AGMouseCheckWidget<AGImage
  attr_reader :mousePos
  def initialize(p,r,s,tile)
    super
    @mousePos=AGVector2.new(0,0)
  end
  def eventMouseMotion(e)
    @mousePos=e.getMousePosition
  end
end


class MyTestApp<AGApplication
  BIGRECT=AGRect2.new(0,0,200,200)
  SMALLRECT=AGRect2.new(0,0,100,100)
  VECTOR=AGVector2.new(100,100)
  BGCOLOR=AGColor.new(0,0,0)
  FGCOLOR=AGColor.new(0xFF,0,0)
  FGCOLOR2=AGColor.new(0,0,0xFF)
  
  attr_reader :surface, :bgsurface, :screenshot, :image, :sWidget
  
  def initialize
    super
    @sWidget=AGScrollingWidget.new(nil,SMALLRECT)
    @sWidget.setClientRect(BIGRECT)
    setMainWidget(@sWidget)
    @surface=makeSurface(FGCOLOR,FGCOLOR2)
    @bgsurface=makeSurface(BGCOLOR)
    @image=AGMouseCheckWidget.new(@sWidget,SMALLRECT,@surface,false)
    @sWidget.addChild(@image)
    
    addHandler(self,:sigFrameFinished,:grabScreenshot)
  end
  
  def makeScreenshot
  	@makeScreenshot=true
  end
  
  protected
  def grabScreenshot(e)
  	if @makeScreenshot
  	  @screenshot=getScreen.screenshotSurface(false)
	  @makeScreenshot=nil
  	end
  	true
  end
  
  private
  def makeSurface(color,c2=nil)
    w=SMALLRECT.width.to_i
    h=SMALLRECT.height.to_i
    s=AGSurface.new(w,h)
    0.upto(w-1){|x|
      0.upto(h-1){|y|
        s.putPixel(x,y,color)
      }
    }
    if c2
      ((w/3)..(2*w/3)).each{|x|
        ((h/3)..(2*h/3)).each{|y|
          s.putPixel(x,y,c2)
        }
      }
    end
    s
  end
end

describe "AGScrollingWidget" do
  describe "drawing" do
    before(:each) do
      @app=makeTestAppClass(MyTestApp).new
    end
    it "should draw the surface normally in init state" do
      @app.step
      @app.makeScreenshot
      @app.step
      screenshot=@app.screenshot
      screenshot.should_not be_nil
      screenshot.getSubSurface(MyTestApp::SMALLRECT).should == @app.surface
    end
    it "shouldn't draw anything when the widget is scrolled to the right lower corner" do
      @app.sWidget.setVector(MyTestApp::VECTOR*0.5)
      @app.makeScreenshot
      @app.step
      s=@app.screenshot
      s.should_not be_nil
      s.getSubSurface(MyTestApp::SMALLRECT).should_not == @app.surface
    end
  
    it "should scale" do
     proj=AGProjection2D.new(MyTestApp::BIGRECT,MyTestApp::SMALLRECT)
     @app.sWidget.setClient(MyTestApp::BIGRECT,proj)
     @app.makeScreenshot
     @app.step
     screenshot=@app.screenshot
     sr=proj.project(MyTestApp::SMALLRECT)
     sub=screenshot.getSubSurface(sr)
     sub.save("xyz.png")
     sub.should be_similarTo(@app.surface.scale(sr.width.to_i,sr.height.to_i),0.001)
    end
  end
  describe "position translation in events" do
    it "should translate" do
    end
  end
end
