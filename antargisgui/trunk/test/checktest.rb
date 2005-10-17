#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# checktest.rb
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

puts "MenuTest"

main=AGMain.new


main.changeRes(800,600,32,false,false)
app=TestApp.new(false)

screen=AGScreenWidget.new
widget=AGCheckBox.new(screen,AGRect.new(100,100,100,100))
screen.addChild(widget)
group=AGRadioGroup.new(screen,AGRect.new(0,0,200,200))
widget2=AGRadio.new(screen,AGRect.new(200,100,100,100))
widget2.setGroup(group)
screen.addChild(widget2)
widget3=AGRadio.new(screen,AGRect.new(200,140,100,100))
widget3.setGroup(group)
screen.addChild(widget3)
app.setMainWidget(screen)#widget)

#main.changeRes(800,600,32,false,true)

app.run

#
