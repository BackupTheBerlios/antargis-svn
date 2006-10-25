#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# edittest.rb
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

require 'ruby/antargislib.rb'
# require 'libantargis'
#require 'sdl'
require 'gui/test/testapp.rb'
require 'gui/test/theme.rb'

include Libantargis

puts "MenuTest"

# main=AGMain.new
# main.initVideo(800,600,32,false,false)

app=TestApp.new()#false)

screen=AGScreenWidget.new
widget=AGEdit.new(screen,AGRect.new(100,100,100,100))
screen.addChild(widget)
app.setMainWidget(screen)#widget)


app.run

#
