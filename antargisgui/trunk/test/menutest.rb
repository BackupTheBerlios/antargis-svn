#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'
require 'testapp.rb'
require 'theme.rb'

include Libantargisruby

puts "MenuTest"

main=AGMain.new

app=TestApp.new(false)

screen=AGScreenWidget.new
widget=AGMenu.new(nil,AGPoint.new(0,0),"testmenu")
widget.addItem("menu1a")
widget.addItem("menu1b")
widget.addItem("menu1c")
s1=widget.addMenu("submenu1")
s2=widget.addMenu("submenu2")
s1.addItem("menu2a")
s1.addItem("menu2b")
s2.addItem("menu2c")
s2.addItem("menu2d")
widget.show(AGPoint.new(0,0))
screen.addChild(widget)
app.setMainWidget(screen)#widget)

app.run

#
