#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'
require 'testapp.rb'
require 'theme.rb'

include Libantargisruby

puts "MenuTest"

main=AGMain.new

app=TestApp.new()#false)

screen=AGScreenWidget.new
widget=AGEdit.new(screen,AGRect.new(100,100,100,100))
screen.addChild(widget)
app.setMainWidget(screen)#widget)

main.changeRes(800,600,32,false,false)

app.run

#
