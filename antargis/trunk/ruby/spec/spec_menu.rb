#if false

Dir.chdir(File.split(__FILE__)[0])

require '../spec_helper.rb'
require 'ruby/gui/testing.rb'
require 'ruby/mainmenu.rb'

describe 'Main-Menu' do
  include GuiTest
  before(:each) do
    @app=makeTestAppClass(AntMenuApp).new
    @app.step
  end
  it "should display credits" do
     widget("ticker").should be_nil
    credits=widget("credits")
    credits.should be_a_kind_of(AGButton)
    credits.click
    widget("ticker").should be_a_kind_of(AntTicker)
    widget("exit").click
    widget("credits").should be_a_kind_of(AGButton)
     widget("ticker").should be_nil
  end
  it "should end when clicking on quit" do
    quit=widget("quit")
    quit.should be_an_instance_of(AGButton)
    @app.step
    lambda {quit.click}.should change(@app,:quitCounter).by(1)
  end
  it "should be possible to select tutorial and this will start this one" do
    tutorialButton=widget("tutorial")
    tutorialButton.should be_an_instance_of(AGButton)
    lambda {tutorialButton.click}.should cross(:startCampaign)
  end
  it "should be possible to select the tutorial in the campaign section and start it" do
    playCampaign(0)
  end
  it "should be possible to select the birth-campaign and start it" do
    playCampaign(1)
  end
  it "should be possible to flip fullscreen" do
    optionsButton=widget("options")
    optionsButton.should be_an_instance_of(AGButton)
    optionsButton.click
    optionsMenu=widget("OptionsMenu")
    optionsMenu.should be_a_kind_of(AGWidget)
    optionsMenu.should be_visible
    fullscreenButton=optionsMenu.child("fullscreen")
    fullscreenButton.should be_an_instance_of(AGButton)
    lambda {fullscreenButton.click}.should change(getVideo,:fullscreen).from(false).to(true)
    lambda {fullscreenButton.click}.should change(getVideo,:fullscreen).from(true).to(false)
  end
  private
  def playCampaign(number)
    campaignButton=widget("campaign")
    campaignButton.should_not be_nil
    campaignButton.click
    menu=widget("CampaignMenu")
    menu.should be_a_kind_of(AGLayout)
    campaignRadio=menu.child("campaign#{number}")
    campaignRadio.should be_a_kind_of(AGRadio)
    campaignRadio.should_not be_checked
    campaignRadio.click
    campaignRadio.should be_checked
    startButton=menu.child("start")
    startButton.should be_a_kind_of(AGButton)
    lambda {startButton.click}.should cross(:startCampaign)
  end
end

#end
