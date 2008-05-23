require 'ruby/gui/testing.rb'

if false
describe "Screenshots" do

  before(:all) do
    @red=AGColor.new(0xFF,0,0)
    @blue=AGColor.new(0,0,0xFF)
    @s=getScreen
  end
     
  it "should be possible to grab backbuffer screenshot with dma" do
    fill(@blue)
    flip
    fill(@red)
    texture=@s.screenshot(false)
    surface=texture.getSurface
    checkSurface(surface, @red).should be_true
  end
  it "should be possible to grab back-buffer screenshot with readpixels" do
	  fill(@blue)
	  flip
	  fill(@red)
    surface=@s.screenshotSurface(false)
    checkSurface(surface, @red).should be_true
  end
  
  it "should be possible to grab front-buffer screenshot with dma" do
    fill(@blue)
    flip
    fill(@red)
    flip
    surface=@s.screenshot(true).getSurface
    checkSurface(surface, @red).should be_true
  end
  it "should be possible to grab front-buffer screenshot with readpixels" do
    fill(@blue)
    flip
    fill(@red)
    flip
    surface=@s.screenshotSurface(true)
    checkSurface(surface, @red).should be_true
  end
  
  private
  
  
  def fill(color)
    @s.fillRect(@s.getRect,color)
  end
  def flip
    @s.flip
  end
  
  def checkSurface(surface,color)
    w=surface.width
    h=surface.height
    equal=true
    (0..10000).each{|i|
      x,y=rand(w),rand(h)
      p=surface.getPixel(x,y)
      if p!=color
        puts p.toString,color.toString 
        equal=false
        break
      end 
    }
    equal
  end
end
end