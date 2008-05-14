require 'ruby/gui/testing.rb'

describe "Screenshots" do
  
  it "should be possible to take screenshots in GL-Mode with AGSurface" do
    getScreen.flip
    black=getScreen.screenshot
    black.width.should == getScreen.getWidth
    black.height.should == getScreen.getHeight
    blackColor=AGColor.new(0,0,0,0)
    0.upto(black.width-1) {|x|
      0.upto(black.height-1){|y|
    #    black.getPixel(x,y).should ==blackColor
      }
    }
    redColor=AGColor.new(0xFF,0,0)
    blueColor=AGColor.new(0,0,0xFF)
    getScreen.fillRect(getScreen.getRect,blueColor)
    getScreen.flip
    getScreen.fillRect(getScreen.getRect,redColor)
    getScreen.flip
    0.upto(black.width-1) {|x|
      0.upto(black.height-1){|y|
        black.getPixel(x,y).should ==blackColor
      }
    }
    redSurface=getScreen.screenshot(true)
    redSurface.save("red.png")
    0.upto(redSurface.width-1) {|x|
      0.upto(redSurface.height-1){|y|
        p redSurface.getPixel(x,y).toString,redColor.toString
        redSurface.getPixel(x,y).should == redColor
      }
    }
    
  end
end