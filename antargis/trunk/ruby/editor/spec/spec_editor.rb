#if false
Dir.chdir(File.split(File.split(File.split(File.split(File.expand_path(__FILE__))[0])[0])[0])[0])
puts Dir.pwd

require 'ruby/gui/testing.rb'

require 'ruby/editor_app.rb'


describe "MapEditor" do
	include GuiTest
	before(:all) do
		@app=makeTestAppClass(AntEditorApp).new(1024,768)
		@mainWidget=@app.getMainWidget
		@app.step
	end
	it "should run" do
		1.should == 1
	end

	it "should create a clear map when using the new-dialog" do
		new=widget("new")
		new.visible.should be_true
		new.click
		dialog=widget("NewDialog")
		dialog.visible.should be_true

		okButton=dialog.child("ok")
		okButton.visible.should be_true
		okButton.click
		@app.step
		widget("NewDialog").valid.should be_false
		@app.step
	end

	it "should add trees to the empty map" do
		entitiesButton=widget("entities")
		entitiesButton.should be_a_kind_of(AGRadio)
		entitiesButton.click
		entitiesList=widget("entitiesTab")
		entitiesList.should be_an_instance_of(AntEntListWidget)
		entitiesList.should be_visible
		fir=entitiesList.child("AntFir")
		fir.should be_an_instance_of(AGRadio)
		fir.isChecked.should be_false
		fir.click
		entitiesList.entType.should == AntFir
		@app.getMap.should have_exactly(0).getAllEntities
		clickScreen(700,200)
		@app.getMap.should have_exactly(1).getAllEntities
		clickScreen(710,210)
		@app.getMap.should have_exactly(2).getAllEntities
	end
	it "should display the player-dialog" do
		playerButton=widget("playerButton")
		playerButton.should be_an_instance_of(AGButton)
		playerButton.click
		playerDialog=widget("PlayersDialog")
		playerDialog.should be_visible
	end

	it "should be ok to save" do


		widget("saveButton").click


		saveDialog=widget("SaveDialog")


		saveDialog.should be_a_kind_of(AGWidget)
		saveDialog.should be_visible
		filename=saveDialog.child("Filename")
		filename.should be_a_kind_of(AGEdit)
		filename.setText(AGStringUtf8.new("mytestfile"))
		okButton=saveDialog.child("ok")
		okButton.should be_a_kind_of(AGButton)
		okButton.click
		File.should be_exists(getWriteDir+"/data/levels/mytestfile.png")
		File.should be_exists(getWriteDir+"/data/levels/mytestfile.antlvl")
	end


end
#end