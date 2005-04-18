#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# tabletest.rb
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
#require 'theme.rb'

include Libantargisruby

def loadAGSurface(s)
	screen=getScreen
	return screen.loadSurface(s)
end

puts "MenuTest"

main=AGMain.new

app=TestApp.new()#false)

screen=AGScreenWidget.new
widget=AGTable.new(screen,AGRect.new(100,100,100,100))
#widget.addFixedRow()
#widget.addFixedRow()
#widget.addFixedColumn()
#widget.addFixedColumn()

widget.addRow(1)
widget.addRow(1)
widget.addColumn(1)
widget.addColumn(1)
#widget.addFixedRow(30)
#widget.addRow(0.5)
#widget.addFixedColumn(30)
#widget.addColumn(0.5)

s1=loadAGSurface("box.png")
s2=loadAGSurface("box_cross.png")

widget.addChild(0,0,AGImage.new(widget,AGPoint.new(0,0),s1,false))
widget.addChild(0,1,AGImage.new(widget,AGPoint.new(0,0),s2,false))
#widget.addChild(0,1,AGImage.new(widget,s2))
#widget.addChild(1,1,AGImage.new(widget,s2))

#widget.addChild(0,0,AGButton.new(widget,AGRect.new(0,0,10,10),"b1"))
#widget.addChild(0,1,AGButton.new(widget,AGRect.new(0,0,10,10),"b2"))
widget.addChild(1,0,AGButton.new(widget,AGRect.new(0,0,24,24),"b3"))
widget.addChild(1,1,AGButton.new(widget,AGRect.new(0,0,24,24),"b4"))

widget.arrange

screen.addChild(widget)
app.setMainWidget(screen)

app.run

#
