#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'
require 'testapp.rb'

include Libantargisruby

puts "MenuTest"

main=AGMain.new

app=TestApp.new(false)

screen=AGScreenWidget.new
widget=AGCheckBox.new(screen,AGRect.new(100,100,100,100),"box1")
screen.addChild(widget)
group=AGRadioGroup.new
widget2=AGRadio.new(screen,AGRect.new(200,100,100,100),group,"box2")
screen.addChild(widget2)
widget3=AGRadio.new(screen,AGRect.new(200,140,100,100),group,"box3")
screen.addChild(widget3)
app.setMainWidget(screen)#widget)

main.changeRes(800,600,32,false,true)

app.run

#
