#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'
require 'testapp.rb'
require 'theme.rb'

include Libantargisruby

main=AGMain.new

app=TestApp.new()#false)

widget=AGVTiler.new(nil,AGRect.new(0,0,5,5))
w1=AGButton.new(widget,AGRect.new(0,0,10,20),"hallo1")
w2=AGButton.new(widget,AGRect.new(10,0,20,20),"hallo2")
w3=AGButton.new(widget,AGRect.new(20,0,40,20),"hallo3")
w4=AGButton.new(widget,AGRect.new(30,0,50,30),"hallo4")
widget.addChild(w1)
widget.addChild(w2)
widget.addChild(w3)
widget.addChild(w4)
app.setMainWidget(widget)

app.run

#
