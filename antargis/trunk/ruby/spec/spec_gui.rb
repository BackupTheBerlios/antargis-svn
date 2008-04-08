require 'ruby/gui/testing.rb'

describe "AGEdit" do
  include GuiTest
  it "should ignore pressing RETURN,ESCAPE when not in multiline mode" do
    @app=makeTestAppClass(AGApplication).new
    widget=AGEdit.new(nil,AGRect2.new(20,20,200,50))
    @app.setMainWidget(widget)
    @app.step
    key('a')
    widget.getText.to_s.should == "a"
    #"it should work".should == ""
    
    #while true; @app.run;end
  end
end

describe "AGListBox" do
  it "should display correctly when a text-entry too long" 
  #do
  #  app=makeTestApp(AGApplication)
  #  widget=AGListBox.new()
  #end
end