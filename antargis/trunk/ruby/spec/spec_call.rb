require 'ruby/spec_helper.rb'

def globalTestFunction
  "myFunc"
end

describe "Call-predicate in spec-helper" do
  it "should work with global functions" do
    globalTestFunction.should == "myFunc"
    lambda {globalTestFunction.should be_nil}.should call(:globalTestFunction)
    globalTestFunction.should == "myFunc"
  end
  it "should work with methods" do
    class A
      def y
        "hi"
      end
    end
    a=A.new
		a.y.should == "hi"
    lambda{a.y.should be_nil}.should call(a,:y)
		a.y.should == "hi"
  end
	it "should work with should_not" do
		lambda{}.should_not call(:globalTestFunction)
	end

	it "should work with new method" do
		class A
			attr_reader :a
			def initialize
				@a=10
			end
		end
		
		lambda{A.new}.should call(A,:new)
		lambda{}.should_not call(A,:new)
		lambda{a=A.new}.should call(A,:new)
	end

end

