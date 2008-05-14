require 'ruby/gui/testing.rb'

describe "AGEdit" do
  include GuiTest
  before(:each) do
    @app=makeTestAppClass(AGApplication).new
    @widget=AGEdit.new(nil,AGRect2.new(20,20,200,50))
    @app.setMainWidget(@widget)
    @app.step
  end
  
  it "should accept RETURN as newline" do
    key('a')
    key(SDLK_RETURN.chr)
    key('a')
    @widget.getText.to_s.should == "a\na"
  end
  
  it "should ignore ESCAPE" do
    key('a')
    key(SDLK_ESCAPE.chr)
    @widget.getText.to_s.should == "a"
  end
  
  it "should ignore pressing RETURN when not in multiline mode" do
    @widget.setMulti(false)
    key('a')
    key(SDLK_RETURN.chr)
    @widget.getText.to_s.should == "a"
  end
end

describe "AGListBox" do
  it "should display correctly when a text-entry too long" 
  #do
  #  app=makeTestApp(AGApplication)
  #  widget=AGListBox.new()
  #end
end

 