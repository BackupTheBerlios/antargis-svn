
require 'ruby/spec/level_testing.rb'

describe "Saving of" do
  include LevelTesting
  describe "RestJob" do
    before(:all) do
      getTestApp(:tutorial0)
      advance
      clickAwayStory
      advance
    end
    it "should be possible to save normal resting" do
      @app.save
      advance
      dialog=widget("SaveDialog")
      dialog.getChild("Filename").setText(AGStringUtf8.new("testfile0"))
      click(dialog.getChild("ok").getScreenRect.getMiddle)
      advance
    end
  end
end