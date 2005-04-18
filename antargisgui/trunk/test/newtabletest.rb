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

require 'libantargisruby'
require 'sdl'
require 'testapp.rb'
require 'theme.rb'

include Libantargisruby

puts "MenuTest"


def loadAGSurface(s)
	screen=getScreen
	return screen.loadSurface(s)
end


main=AGMain.new

main.changeRes(640,480,32,false,true)

app=TestApp.new(false)

t=AGTable.new(nil,AGRect.new(50,50,200,200))
t.addRow(1)
t.addRow(1)
t.addColumn(1)
t.addColumn(1)

t.addChild(0,0,AGButton.new(t,AGRect.new(0,0,20,20),"hupe"))
t.addChild(1,0,AGButton.new(t,AGRect.new(0,0,20,20),"hupe"))
t.addChild(0,1,AGButton.new(t,AGRect.new(0,0,20,20),"hupe"))
#t.addChild(1,1,AGButton.new(t,AGRect.new(0,0,20,20),"hupe"))
s1=loadAGSurface("box_cross.png")
t.addChild(1,1,AGImage.new(t,AGPoint.new(0,0),s1,false))

t.arrange
app.setMainWidget(t)
app.run
