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
		surface=AGSurface.new(64,64)
		
		for x in 0..31
			for y in 0..31
				surface.putPixel(x,y,AGColor.new(0xFF,0,0))
				surface.putPixel(x+32,y+32,AGColor.new(0,0xFF,0))
			end
		end
		@tex=AGTexture.new(surface)
		#return
		#@tex=AGTexture.new(64,64) #getTextureManager.makeTexture(surface)
		#@tex=AGTexture.new(surface)
		#return
		@tex.beginPaint
		#@tex.blit(test,AGRect.new(0,0,128,128),AGRect.new(0,0,128,128))
		#@tex.endPaint
		#return
		for x in 32..63
			for y in 0..31
				@tex.putPixel(x,y,AGColor.new(0,0,0xFF,0xFF))
			end
		end
		for x in 0..15
			for y in 0..15
				@tex.putPixel(x,y,AGColor.new(0xFF,0xFF,0xFF,0xFF))
			end
		end
		@tex.endPaint
	end
	def draw(p)
		p.fillRect(AGRect.new(0,0,65,65),AGColor.new(0xFF,0,0xFF))
		p.fillRect(AGRect.new(0,0,64,64),AGColor.new(0xFF,0xFF,0))
		p.blit(@tex,@tex.getRect.origin)
		p.putPixel(AGPoint.new(0,0),AGColor.new(0,0,0))
	end
end

main=AGMain.new(640,480,32,false,true)

app=TestApp.new(false)

m=MyWidget.new(nil,AGRect.new(1,1,500,500))
app.setMainWidget(m)
app.run
