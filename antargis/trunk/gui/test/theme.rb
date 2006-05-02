#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# theme.rb
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

def loadAGSurface(s)
	screen=getScreen
	return screen.loadSurface(s)
end

class MyTheme<AGTheme
	def initialize()
		super
		initColors
	end
	def getSurface(s)
		puts "getSurfaceruby:"+s
		return AGSurface.new
	end
		
	def initColors()
		setColor("borderColor1",AGColor.new(0xE0,0xE0,0xE0,0xFF))
		setColor("borderColor2",AGColor.new(0x40,0x40,0x40))
		
		setColor("gradientColor1",AGColor.new(0xE7,0xE7,0xE7))
    setColor("gradientColor2",AGColor.new(0xC9,0xC9,0xC9))
    setColor("gradientColor3",AGColor.new(0xCC,0xCC,0xCC))
    setColor("gradientColor4",AGColor.new(0xAE,0xAE,0xAE))
		setColor("gradientColor1.normal",AGColor.new(0xE7,0xE7,0xE7))
    setColor("gradientColor2.normal",AGColor.new(0xC9,0xC9,0xC9))
    setColor("gradientColor3.normal",AGColor.new(0xCC,0xCC,0xCC))
    setColor("gradientColor4.normal",AGColor.new(0xAE,0xAE,0xAE))
    setColor("gradientColor1.lighted",AGColor.new(0xF5,0xE1,0xFF))
    setColor("gradientColor2.lighted",AGColor.new(0xDC,0xC8,0xFF))
    setColor("gradientColor3.lighted",AGColor.new(0xDC,0xC8,0xFF))
    setColor("gradientColor4.lighted",AGColor.new(0xC3,0xAF,0xFF))
    setColor("gradientColor1.pressed",AGColor.new(0xCF,0xAF,0xFF))
    setColor("gradientColor2.pressed",AGColor.new(0xDC,0xC8,0xFF))
    setColor("gradientColor3.pressed",AGColor.new(0xDC,0xC8,0xFF))
    setColor("gradientColor4.pressed",AGColor.new(0xF5,0xE1,0xFF))
		
		setColor("gradientColor1.menu",AGColor.new(0xE7,0xE7,0xE7,0xFF))
		setColor("gradientColor2.menu",AGColor.new(0xE7,0xE7,0xE7,0xFF))
		setColor("gradientColor3.menu",AGColor.new(0xE7,0xE7,0xE7,0x7f))
		setColor("gradientColor4.menu",AGColor.new(0xE7,0xE7,0xE7,0x7f))
    setColor("gradientColor1.menu.lighted",AGColor.new(0xa5,0xa1,0xFF,0xFF))
    setColor("gradientColor2.menu.lighted",AGColor.new(0xa5,0xa1,0xFF,0xFF))
    setColor("gradientColor3.menu.lighted",AGColor.new(0xa5,0xa1,0xFF,0x7f))
    setColor("gradientColor4.menu.lighted",AGColor.new(0xa5,0xa0,0xFF,0x7f))
		
		setSurface("backgroundSurface.screen",loadAGSurface("bigbg_tile.png"))#bg_tile.png"))
		setSurface("backgroundSurface.menu",loadAGSurface("bg_tile2.png"))
		setSurface("window_border",loadAGSurface("win_border.png"))
		setSurface("close_button",loadAGSurface("close_button.png"))
		
		setSurface("checkBox.normal",loadAGSurface("box.png"))
		setSurface("checkBox.checked",loadAGSurface("box_cross.png"))
		
		setSurface("radioButton.normal",loadAGSurface("round.png"))
		setSurface("radioButton.checked",loadAGSurface("round_checked.png"))
		
		menuFont=AGFont.new("Arial.ttf",14)
		menuFont.setColor(AGColor.new(0,0,0))
		setFont("Font.menu",menuFont)
		
		setFont("Font.checkBox",menuFont)
		setFont("Font.radioButton",menuFont)
		setFont("Font.windowTitle",menuFont)
		
		setInt("borderWidth.button",1)

		puts "initColors"
		 #AGColor bc1(0xE0,0xE0,0xE0);
  #AGColor bc2(0x40,0x40,0x40);


		
		#puts "getting Color:"+name
		#return AGColor.new(0,0,255)
	end
#	def getFont(name)
#		return AGFont.new("Arial.ttf")
#	end
end

def initTheme
	Libantargisruby.setTheme(MyTheme.new())
end
