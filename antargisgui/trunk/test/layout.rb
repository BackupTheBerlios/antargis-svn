#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'
require 'testapp.rb'
require 'theme.rb'

include Libantargisruby

puts "MenuTest"

main=AGMain.new

main.changeRes(640,480,32,false,true)

app=TestApp.new(false)

screen=AGScreenWidget.new
window1=AGWindow.new(screen,AGRect.new(100,100,200,100),"test Window")
window2=AGWindow.new(screen,AGRect.new(150,150,200,100))

screen.addChild(window1)
screen.addChild(window2)

w2=AGButton.new(window1.getClient(),AGRect.new(10,10,40,30),"hallo")
window1.addChild(w2)

doc=Document.new("dialog.xml")
puts doc.toString
l=AGLayout.new(screen,doc.toString)
screen.addChild(l)



app.setMainWidget(screen)#widget)

app.run

#
