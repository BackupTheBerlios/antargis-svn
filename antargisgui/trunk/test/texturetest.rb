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

class MyWidget<AGWidget
	def initialize(p,r)
		super
		surface=AGSurface.new(128,128)
		for x in 0..64
			for y in 0..64
				surface.putPixel(x,y,AGColor.new(0xFF,0,0))
				surface.putPixel(x+64,y+64,AGColor.new(0,0xFF,0))
			end
		end
		@tex=getTextureManager.makeTexture(surface)
		#return
		@tex.beginPaint
		for x in 0..32
			for y in 0..32
				@tex.putPixel(x,y,AGColor.new(0,0,0xFF))
			end
		end
		@tex.endPaint
	end
	def draw(p)
		p.drawRect(AGRect.new(0,0,130,130),AGColor.new(0,0,0))
		p.blit(@tex,@tex.getRect.origin)
	end
end

main=AGMain.new(640,480,32,false,true)

app=TestApp.new(false)

sc=AGScreenWidget.new

sc.addChild(MyWidget.new(sc,AGRect.new(0,0,500,500)))
app.setMainWidget(sc)
app.run
