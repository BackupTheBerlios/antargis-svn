#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# wintestgl.rb
# by David Kamphausen (david.kamphausen@web.de)
#
# The "Antargis" project, including all files needed to compile it,
# is free software; you can redistribute it and/or use it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 2 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# You should have received a copy of the GNU General Public
# License along with this program.
#

#!/usr/bin/ruby

require 'libantargisruby.dll'
#require 'sdl'
require 'testapp.rb'
require 'theme.rb'

include Libantargisruby

puts "MenuTest"

main=AGMain.new

main.changeRes(640,480,32,false,true)

app=TestApp.new(false)

screen=AGScreenWidget.new
#window1=AGWindow.new(screen,AGRect.new(100,100,200,100),"test Window")
window2=AGWindow.new(screen,AGRect.new(150,150,200,100),"test")
#b=AGButton.new(screen,AGRect.new(0,0,200,100),"muh")
#screen.addChild(b)
#b2=AGButton.new(window2,AGRect.new(0,0,10,10),"h")
#window2.addChild(b2)
window2.addChild(AGEdit.new(window2,AGRect.new(5,20,100,100)))

#screen.addChild(window1)
screen.addChild(window2)

#w2=AGButton.new(window1.getClient(),AGRect.new(10,10,40,30),"hallo")
#window1.addChild(w2)

app.setMainWidget(screen)#widget)

app.run

#
