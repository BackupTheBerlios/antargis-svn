describe "Dependencies" do
  describe "for swig" do
    it "should handle interface-files correctly"
    it "should handle depending header files correctly"
    it "the compiler should be called"
  end
  describe "for gcc" do
    it "should handle c-files correctly"
    it "should handle depending header files correctly"
  end
end

describe "Config" do
  it "should have an interface for configuring profiling, debugging, paths and so on"
  it "should handle config-scripts in correct order"
  it "should find libraries"
  it "should find programs"
  it "should check libraries for functions"
  it "should have an option for using mingw"
end

describe "Interface generation" do
  it "should generate directors for all ruby-classes"
end