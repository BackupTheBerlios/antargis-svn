#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'
require 'testapp.rb'
include Libantargisruby


#class MyApp <AGApplication
#	def eventQuit(event)
#		puts "Quitting"
#		super(event)
#	end
#end

main=AGMain.new

app=TestApp.new

widget=AGButton.new(nil,AGRect.new(0,0,50,20),"hallo")
w2=AGButton.new(widget,AGRect.new(0,20,50,20),"hallo")
w3=AGButton.new(widget,AGRect.new(0,40,50,20),"hallo")
w4=AGButton.new(widget,AGRect.new(0,60,50,20),"hallo")
w5=AGButton.new(widget,AGRect.new(0,80,50,20),"hallo")
widget.addChild(w2)
widget.addChild(w3)
widget.addChild(w4)
widget.addChild(w5)
app.setMainWidget(widget)

app.run

#