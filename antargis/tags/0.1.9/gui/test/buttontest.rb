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

require 'antargisgui.rb'
require 'testapp.rb'
require 'theme.rb'

puts "MenuTest"

#main=AGMain.new(640,480,32,false,true)

main=AGMain.new(640,480,32,false,false)

app=TestApp.new(false)

sc=AGScreenWidget.new

button=AGButton.new(sc,AGRect.new(0,0,200,100),"text")
b2=AGButton.new(button,AGRect.new(10,10,60,40),"text2")
b2.setTheme("antButton")
b3=AGButton.new(button,AGRect.new(100,10,60,40),"text2")
b3.setTheme("yellowButton")
#button.setText("huhu")
button.addChild(b2)
button.addChild(b3)
sc.addChild(button)
app.setMainWidget(sc)
app.run
