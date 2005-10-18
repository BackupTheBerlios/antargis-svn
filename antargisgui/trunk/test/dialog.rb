#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# layout.rb
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

class TestApp <AGApplication
	def initialize(autoexit=true)
		@count=0
		@autoexit=autoexit
		super()
	end
	def eventQuit(event)
		puts "Quitting"
		super(event)
	end
	def eventIdle
		@count+=1
		if @count>=20 then
			if @autoexit then
				puts "correct quit after 20 idles"
				tryQuit
			end
		end
	end
end

main=AGMain.new

doc=Document.new("theme.xml")
loadTheme(doc.toString)

main.changeRes(640,480,32,false,true)

app=TestApp.new(false)

screen=AGScreenWidget.new

doc=Document.new("../data/dialog.xml")
puts doc.toString
l=AGDialog.new(screen,doc.toString)
screen.addChild(l)

app.setMainWidget(screen)

app.run

