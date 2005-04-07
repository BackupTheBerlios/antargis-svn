#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# tiletest.rb
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

#!/usr/bin/env ruby

require 'libantargisruby'
include Libantargisruby

main=AGMain.new

class MyWidget< AGWidget
	def initialize(p,r)
		super(p,r)
		@surface=getScreen().loadSurface("win_border.png")
	end
	def draw(r)
		getScreen().tile(@surface,AGRect.new(10,10,50,8),AGRect.new(8,0,8,8))
	end
end

app=AGApplication.new

w=MyWidget.new(nil,AGRect.new(0,0,0,0))

app.setMainWidget(w)

app.run