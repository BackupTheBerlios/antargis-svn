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
app.setMainWidget(screen)

#main.changeRes(640,480,32,false,true)

app.run

#
