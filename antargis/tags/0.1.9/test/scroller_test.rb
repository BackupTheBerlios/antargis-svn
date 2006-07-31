#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# game.rb
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

$programDir=Dir.pwd+"/ruby"
# add programdir to path
$:.push($programDir)

require 'antargislib.rb'

require 'dialogs.rb'
require 'ents.rb'
require 'map.rb'

class AntGameApp<AGApplication
	def initialize()
		super()
		#super(w,h)
		s=AGScreenWidget.new
		setMainWidget(s)
# 		sc=AGScroller.new(s,AGRect2.new(10,10,200,32),true)
# 		s.addChild(sc)
# 		sc2=AGScroller.new(s,AGRect2.new(10,50,24,100),false)
# 		s.addChild(sc2)

		l=AGListBox.new(s,AGRect2.new(100,100,100,200))
		s.addChild(l)
		(0..20).each{|c|l.insertItem(c.to_s,c.to_s)}
		l.updateScroller

	end
	def eventFrame(t)
		delay(20)
	end
end


#$main=AGMain.new

#$main.initVideo(1024,768,32,false,true)
#main.changeRes(1400,1050,32,true,true)

app=AntGameApp.new

app.run

puts "huhu"

