#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'
require 'testapp.rb'

include Libantargisruby



main=AGMain.new

app=TestApp.new

r=AGRect.new(100,100,50,20)
r2=AGRect.new(0,100,50,20)
f=AGFont.new("Arial.ttf")
widget=AGText.new(nil,r,"hallo",f)
widget2=AGText.new(widget,r2,"hallo",AGFont.new("Arial.ttf"))
widget.addChild(widget2)
widget2.setDeriveRect

widget.sigClick.connect(app)

app.setMainWidget(widget)

app.run

#