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
widget=AGTable.new(screen,AGRect.new(100,100,100,100))
#widget.addFixedRow()
#widget.addFixedRow()
#widget.addFixedColumn()
#widget.addFixedColumn()

widget.addFixedRow()
widget.addRow(0.5)
widget.addFixedColumn()
widget.addColumn(0.5)

s1=loadAGSurface("box.png")
s2=loadAGSurface("box_cross.png")

widget.addChild(0,0,AGImage.new(widget,s1,false))
widget.addChild(0,1,AGImage.new(widget,s2,false))
#widget.addChild(0,1,AGImage.new(widget,s2))
#widget.addChild(1,1,AGImage.new(widget,s2))

#widget.addChild(0,0,AGButton.new(widget,AGRect.new(0,0,10,10),"b1"))
#widget.addChild(0,1,AGButton.new(widget,AGRect.new(0,0,10,10),"b2"))
widget.addChild(1,0,AGButton.new(widget,AGRect.new(0,0,24,24),"b3"))
widget.addChild(1,1,AGButton.new(widget,AGRect.new(0,0,24,24),"b4"))

widget.arrange

screen.addChild(widget)
app.setMainWidget(screen)#widget)

app.run

#
