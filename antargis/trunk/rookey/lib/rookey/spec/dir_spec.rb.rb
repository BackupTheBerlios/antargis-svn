require File.expand_path(__FILE__,"../compiler.rb")

describe "Compiler directories" do
  it "should ignore relativ parent paths" do
    Compiler.makePlainName("../a.cc").should == "..=a.cc"
    Compiler.makePlainName("xy/a.cc").should == "xy=a.cc"
    Compiler.makePlainName("hupe.cc").should ==" hupe.cc"
  end
end