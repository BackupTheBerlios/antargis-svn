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
#require 'sdl'
require 'testapp.rb'
require 'theme.rb'

include Libantargisruby

puts "MenuTest"

main=AGMain.new

main.changeRes(640,480,32,false,true)

app=TestApp.new(false)

sc=AGScreenWidget.new

lb=AGListBox.new(sc,AGRect.new(0,0,200,100))

for i in 1..20 do
	lb.insert("hallo1",i.to_s)
end


sc.addChild(lb)
app.setMainWidget(sc)


app.run
