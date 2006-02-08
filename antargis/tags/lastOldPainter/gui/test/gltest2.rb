#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# testapp.rb
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
include Libantargisruby

class MyWidget<AGWidget
	def initialize(p,r)
		super(p,r)
		@s=getSurfaceManager.loadSurface("face1.png")
		@t=getTextureManager.makeTexture(@s)
	end
	def draw(p)
		p.blit(@t,AGRect.new(0,0,100,100))
	end
end

class TestApp <AGApplication
	def initialize(autoexit=true)
#		initTheme
		@count=0
		@autoexit=autoexit
		super()
		setMainWidget(MyWidget.new(nil,AGRect.new(0,0,200,200)))
	end
	def eventQuit(event)
		puts "Quitting"
		super(event)
	end
	def eventIdle
		delay(1000)
	end
	def eventIdle2
		#puts "idle count:"+@count.to_s
		@count+=1
		if @count>=20 then
			if @autoexit then
				puts "correct quit after 20 idles"
				tryQuit
			end
		end
	end
end

$main=AGMain.new
$main.changeRes(800,600,32,false,true)
$app=TestApp.new
$app.run
