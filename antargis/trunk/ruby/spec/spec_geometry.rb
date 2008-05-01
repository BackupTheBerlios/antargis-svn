require 'ruby/antargislib.rb'
require 'pp'

describe AGCircle2,"a radius 2 circle around (10,10)" do
	before(:each) do
		@circleMiddle=AGVector2.new(10,10)
		@radius=2
		@circle=AGCircle2.new(@circleMiddle,@radius)
	end

	it "should hit an intersection line (9,0)-(9,20)" do
		line=AGLine2.new(AGVector2.new(9,0),AGVector2.new(9,20))
		list=@circle.collide(line)
		list.length.should == 2
		list.each{|p|(p-@circleMiddle).length.should == @radius}

		# should lie on line
		list.each{|p|p.x.should ==9}

	end
	it "should not hit an line at (9,0)-(119,0)" do
		l2=AGLine2.new(AGVector2.new(9,0),AGVector2.new(119,0))
		list=@circle.collide(l2)
		list.length.should ==0

	end
  
end

describe "a triangle with coord (10,10),(10,100),(20,100)" do
  it "should contain (15,80)" do
    t=AGTriangle2.new(AGVector2.new(10,10),AGVector2.new(10,100),AGVector2.new(20,100))
    v=AGVector2.new(15,80)
    pp t,v
    t.contains(v).should be_true
  end
  it "should not contain (15,20)" do
    t=AGTriangle2.new(AGVector2.new(10,10),AGVector2.new(10,100),AGVector2.new(20,100))
    v=AGVector2.new(15,20)
    pp t,v
    t.contains(v).should be_false
  end
end

describe "a triangle with coord (128.435,222.575) (148,243) (168.425,223.435)" do
  it "should contains (142,243)" do
    t=AGTriangle2.new(AGVector2.new(128.435,222.575),AGVector2.new(148,243),AGVector2.new(168.425,223.435))
    
    if false
    p=AGPainter.new(getScreen)
    p.fillRect(p.getRect,AGColor.new(0,0,0))
    r=AGRect2.new(0,0,1,1)
    (0..200).step(2){|x|
      (0..300).step(2){|y|
        #if x
        v=AGVector2.new(x,y)
        c=AGColor.new(0xFF,0,0)
        c=AGColor.new(0,0,0xFF) if t.contains(v)
        #p.putPixel(v,c)
        p.fillRect(r+v,c)
      }
    }
    getScreen.flip
    getMain.delay(10000)
    end
    
    v=AGVector2.new(148,240)
    t.contains(v).should be_true
  end
end


describe "a Rectangle of (0,0),(2048,1024)" do
  before(:all) do
    @r=AGRect2.new(0,0,2048,1024)
  end
  it "should not clip (10,10)" do
    v=AGVector2.new(10,10)
    @r.clip(v).should == v
  end
end