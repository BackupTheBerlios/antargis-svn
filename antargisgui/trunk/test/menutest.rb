#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# menutest.rb
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
