#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# apptest.rb
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
include Libantargisruby


#class MyApp <AGApplication
#	def eventQuit(event)
#		puts "Quitting"
#		super(event)
#	end
#end

main=AGMain.new

app=TestApp.new

widget=AGButton.new(nil,AGRect.new(0,0,50,20),"hallo")
w2=AGButton.new(widget,AGRect.new(0,20,50,20),"hallo")
w3=AGButton.new(widget,AGRect.new(0,40,50,20),"hallo")
w4=AGButton.new(widget,AGRect.new(0,60,50,20),"hallo")
w5=AGButton.new(widget,AGRect.new(0,80,50,20),"hallo")
widget.addChild(w2)
widget.addChild(w3)
widget.addChild(w4)
widget.addChild(w5)
app.setMainWidget(widget)

app.run

#