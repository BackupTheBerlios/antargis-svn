require 'ruby/spec_helper.rb'

def globalTestFunction
  "myFunc"
end

describe "Call-predicate in spec-helper" do
  it "should work with global functions" do
    globalTestFunction.should == "myFunc"
    lambda {globalTestFunction.should be_nil}.should cross(:globalTestFunction)
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
    lambda{a.y.should be_nil}.should cross(a,:y)
    a.y.should == "hi"
  end
  it "should work with should_not" do
    lambda{}.should_not cross(:globalTestFunction)
  end

  it "should work with new method" do
    class A
      attr_reader :a
      def initialize
        @a=10
      end
    end
    
    lambda{A.new}.should cross(A,:initialize)
    lambda{}.should_not cross(A,:initialize)
    lambda{a=A.new}.should cross(A,:initialize)
  end

end




def myGlobalFunction
  :test1
end

def myGlobalCaller
  myGlobalFunction
end

class Test
  def initialize
    puts "new Test"
  end
  def Test.test2
    :test_2
  end
  def test3
    :test_3
  end
end

describe "Cross" do
  it "should work for global functions" do
    myGlobalCaller.should equal(:test1)
    lambda{myGlobalCaller.should be_nil}.should cross(:myGlobalFunction)
    myGlobalCaller.should equal(:test1)
  end
  
  it "should work for class-methods" do
    lambda{Test.test2}.should cross(Test,:test2)
    Test.test2.should equal(:test_2)
  end
  
  it "should work for object-methods" do
    t=Test.new
    t.test3.should equal(:test_3)
    lambda{t.test3}.should cross(t,:test3)
    t.test3.should equal(:test_3)
  end
  
  it "should work with new on global class,too" do
    lambda{Test.new}.should cross(Test,:initialize)
  end

  it "failure measure" do
    Cross.new("A","B").failure_message.should =~/expected .* to call A.B.*/
  end
  it "negative failure measure" do
    Cross.new("A","B").negative_failure_message.should =~/expected .* not to call A.B.*/
  end
  
  it "should work with local classes" do
    class A
      attr_reader :x
      def initialize
        @x=20
      end
      def y
        10
      end
      def A.z        
        :az
      end
    end
    a=nil
    a=A.new
    lambda {a.y}.should cross(a,:y)
    lambda {a=A.new}.should cross(A,:initialize)
    A.new.y.should equal(10)
    lambda {}.should_not cross(A,:initialize)
    
    lambda{A.z}.should cross(A,:z)
    lambda{}.should_not cross(A,:z)
    A.z.should equal(:az)
  end
end

