#!/usr/bin/env ruby
require 'test/unit'
require 'ruby/gui/testing.rb'

Dir.chdir(File.split(File.split(File.split(File.split(File.expand_path(__FILE__))[0])[0])[0])[0])
puts Dir.pwd

require 'ruby/editor_app.rb'


class DialogTest<Test::Unit::TestCase
  include GuiTest
  def setup
    @app=makeTestAppClass(AntEditorApp).new(1024,768)
    @mainWidget=@app.getMainWidget
    @app.step
  end
  def test_simplerun
    newdialog
    addtree
     playerdialog
  end
private
   def checkGlobals
     assert($map.nil?,"$map is nil")
   end
  def newdialog
    new=widget("new")
    assert(new.visible)
    new.click
    dialog=widget("NewDialog")
    assert(dialog.visible,"NewDialog is not visible after clicking 'new'")

    okButton=dialog.child("ok")
    assert(okButton.visible,"ok-button in NewDialog is not visible")
    okButton.click
    @app.step
    assert((not widget("NewDialog").valid),"new dialog is not registered anymore")
    @app.step
  end
  def addtree
    entitiesButton=widget("entities")
    assert(entitiesButton.is_a?(AGRadio),"Button named 'entities' is a radio-button")
    entitiesButton.click
    entitiesList=widget("entitiesTab")
    assert(entitiesList.is_a?(AntEntListWidget),"Entity List is named 'entitiesTab'")
    assert(entitiesList.visible,"entity list is visible")
    fir=entitiesList.child("AntFir")
    assert(fir.is_a?(AGRadio),"fir button is a radio-button")
    assert((not fir.isChecked),"Fir button is not checked")
    fir.click
    assert(entitiesList.entType==AntFir,"Fir button is checked")
    clickScreen(700,200)
    assert(@app.getMap.getAllEntities.length==1,"there's one entity!")
    clickScreen(710,210)
    assert(@app.getMap.getAllEntities.length==2,"there're two entities!")
  end
  def playerdialog
    playerButton=widget("playerButton")
    assert(playerButton.is_a?(AGButton),"playerButton is a button")
    playerButton.click
    playerDialog=widget("PlayersDialog")
    assert(playerDialog.visible,"player dialog is visible")
  end

  def clickScreen(x,y)
    @app.eventMouseButtonDown(newEvent(@app,"",toSDLEvent("SDL_MOUSEBUTTONDOWN:0:1:1:#{x.to_i}:#{y.to_i}")))
    @app.eventMouseButtonUp(newEvent(@app,"",toSDLEvent("SDL_MOUSEBUTTONUP:0:1:1:#{x.to_i}:#{y.to_i}")))
  end
end

