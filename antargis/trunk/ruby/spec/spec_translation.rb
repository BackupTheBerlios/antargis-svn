require 'ruby/spec/level_testing.rb'

describe "Checking translation" do
  
  include LevelTesting
  
  before(:all) do
    getTestApp(:tutorial0)
    advance
  end
  
  it "should diplay normal text and special chars like backslashes" do
    1.should == 0
  end
end