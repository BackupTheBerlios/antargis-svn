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
window1=AGWindow.new(screen,AGRect.new(100,100,200,100),"test Window")
screen.addChild(window1)
window1=AGWindow.new(screen,AGRect.new(150,150,200,100))
screen.addChild(window1)
window1=AGWindow.new(screen,AGRect.new(150,150,200,100))
screen.addChild(window1)
window1=AGWindow.new(screen,AGRect.new(150,150,200,100))
screen.addChild(window1)
window1=AGWindow.new(screen,AGRect.new(150,150,200,100))
screen.addChild(window1)
window1=AGWindow.new(screen,AGRect.new(150,150,200,100))
screen.addChild(window1)
window1=AGWindow.new(screen,AGRect.new(150,150,200,100))
screen.addChild(window1)
window1=AGWindow.new(screen,AGRect.new(150,150,200,100))
screen.addChild(window1)
window1=AGWindow.new(screen,AGRect.new(150,150,200,100))
screen.addChild(window1)

w2=AGButton.new(window1.getClient(),AGRect.new(10,10,40,30),"hallo")
window1.addChild(w2)

a={}
for i in 0..10000
	a[i]=i*i
end

GC.start

app.setMainWidget(screen)#widget)

app.run

#
