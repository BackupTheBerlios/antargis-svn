
require 'ruby/spec/level_testing.rb'

describe "Saving and loading of" do
  SAVEGAME="testfile0"
  SAVEGAME2="testfile1"
  
  include LevelTesting
  describe "RestJob" do
    before(:all) do
      getTestApp(:tutorial0)
      advance
      clickAwayStory
      advance
    end
    it "should be possible to save normal resting" do
      saveText=save(SAVEGAME)
      puts saveText
      load(SAVEGAME)
      newText=save(SAVEGAME2)
      puts newText
      #newText.should == saveText
      exit
      #advance
    end
  end
  
  private
  def checkRowenRest
    #@app.getMap.getByName("Rowen").getJobName.should =="restJob"
  end
  
  # saves current game and returns savegame-text
  def save(file)
    checkRowenRest
    @app.save
    #advance
    dialog=widget("SaveDialog")
    dialog.getChild("Filename").setText(AGStringUtf8.new(file))
    click(dialog.getChild("ok").getScreenRect.getMiddle)
    getSaveText(file)
    checkRowenRest
  end
  
  def load(file)
    checkRowenRest
    #advance
    lambda {
	    @app.load
	    dialog=widget("LoadDialog")
	    files=dialog.getChild("Files")
	    filename=files.getValues.keys.select{|f|f=~/#{file}/}[0]
	    files.selectItem(filename)
	    click(dialog.getChild("ok").getScreenRect.getMiddle)
    }.should_not cross(AntNewHLJob,:check)
    checkRowenRest
  end
  
  def getSaveText(file)
    file="savegames/"+file+".antlvl"
    pp file
    loadFile(file)
  end
end