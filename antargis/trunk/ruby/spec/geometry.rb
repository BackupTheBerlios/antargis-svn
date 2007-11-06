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