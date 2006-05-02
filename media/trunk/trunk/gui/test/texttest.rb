#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# texttest.rb
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

include Libantargisruby



main=AGMain.new

app=TestApp.new

r=AGRect.new(100,100,50,20)
r2=AGRect.new(0,100,50,20)
f=AGFont.new("Arial.ttf")
widget=AGText.new(nil,r,"hallo",f)
widget2=AGText.new(widget,r2,"hallo",AGFont.new("Arial.ttf"))
widget.addChild(widget2)
widget2.setDeriveRect

widget.sigClick.connect(app)

app.setMainWidget(widget)

app.run

#
