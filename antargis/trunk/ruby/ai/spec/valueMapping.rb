require 'pp'
require 'ruby/ai/value_map.rb'

describe "mapValue01 - mapping with min=5" do
	before(:all) do
		@min=5
	end
	it "should map to 0 when parameter is below min" do
		mapValue01(0,@min).should == 0
		mapValue01(3,@min).should == 0
		mapValue01(4.5,@min).should == 0
	end

	it "5 in interval (0,5) should be above 0.99" do
		mapValue01(5,0,5).should > 0.99
	end

	it "should map to a value above 0.99 when parameter is really big (1000) in standard scale" do
		mapValue01(1000,@min).should > 0.99
	end

	it "should be ascending whatsoever" do
		arr=[5,10,20,100,200,250,1000,20000]
		last=0
		arr.each{|v|
			new=mapValue01(v,@min)
			new.should > last
			last=new
		}
	end

	it "should not map above 1 - even with 1000000000000" do
		v=mapValue01(1000000000000,@min).should <= 1
	end
	
end


describe "mapMinValues - mapping array to min" do
	it "should map [1,4,5] to the same value regardless of the order" do
		as=[[1,4,5],[1,5,4],[5,4,1],[5,1,4],[4,1,5],[4,5,1]]
		a=as[0]
		v=mapMinValues(a)
		as.each{|ca|
			mapMinValues(ca).should == v
		}
	end

	it "should map [1,5,5] below [2,5,5] (ascension)" do
		mapMinValues([1,5,5]).should < mapMinValues([2,5,5])
	end
	it "should map [5,5,5] same as [5,5] (regardless the count)" do
		mapMinValues([5,5,5]).should == mapMinValues([5,5])
	end
	it "should map [2,3,5] below [2,4,4] (no averaging - bad min worthens the result)" do
		mapMinValues([2,3,5]).should < mapMinValues([2,4,4])
	end
end