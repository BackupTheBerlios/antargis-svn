#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_server.rb
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


$programDir=Dir.pwd+"/ruby"
# add programdir to path
$:.push($programDir)

$serverDir=Dir.pwd+"/server"
# add programdir to path
$:.push($serverDir)

require 'antargislib.rb'
require 'server.rb'

class AntServer<AGApplication
	def initialize
		super
		@layout=AGLayout.new(nil)
		@layout.loadXML(loadFile("data/gui/layout/server.xml"))
		setMainWidget(@layout)
	end
end


def startServer
	app=AntServer.new
	#app.disableGC
	app.run
end

startServer