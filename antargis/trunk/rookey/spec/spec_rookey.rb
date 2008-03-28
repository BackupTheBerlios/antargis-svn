require File.join(File.split(__FILE__)[0],"helper_spec.rb")

Dir.chdir(File.split(__FILE__)[0])



describe "Dependencies" do
  describe "for swig" do
    it "should handle interface-files correctly"
    it "should handle depending header files correctly"
    it "the compiler should be called"
  end
  describe "for gcc" do
    it "should handle c-files correctly"
    it "should handle depending header files correctly" do
      exe=nil
      rakefile=rake do
        exe=link_exe(compile("main.cc"))
      end
      rakefile.default
      checkExe(local(exe))
      inspectFile(local(exe)) do
        touch("main.h")
        rakefile.default
      end.should be_touched
    end
    it "link should include cleanup" do
      exe=nil
      rakefile=rake do
        exe=link_exe(compile("main.cc"))
      end
      rakefile.default
      File.exists?(local(exe)).should be_true
      rakefile.clean
      File.exists?(local(exe)).should be_false
    end
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
  it "should generate directors for all ruby-classes" do
    dll=nil
    rakefile=rake do 
      dll=link_dll(compile(["main.cc",swig("a.h")]))
    end
    rakefile.default
    # run my test
    ruby do
      c "require '#{local(dll)}"
      c "B.new"
    end
    
    rakefile.clean
  end
end


describe "Swig tests" do
end

