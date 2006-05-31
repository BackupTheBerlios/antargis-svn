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
require 'server/config.rb'
require 'server.rb'
require 'ents.rb'
require 'map.rb'


class AntServer<AGApplication
	def initialize
		super
		@layout=AGLayout.new(nil)
		@layout.loadXML(loadFile("data/gui/layout/server.xml"))
		setMainWidget(@layout)

		@scene=Scene.new(128,128)
		@map=AntRubyMap.new(@scene,128,128)
		$map=@map
		@map.loadMap("levels/multiplayer/multimap1.antlvl")

		table=SimpleLoginTable.new
		table.addPlaintext("muh","puh")
		@server=LoginServer.new(table,self)
		@queue=Queue.new
	end
	def eventFrame(t)
		delay(100)
		@map.move(t)

		# do sync-calls
		while @queue.length>0
			b=@queue.pop
			b.call
		end

	end
	def makeNewHeroMessage(player,hero)
		NewPlayerMessage.new(player.getName,hero.getPos2D)
	end
	def makeWelcomeMessage(name,c)
		puts "makeW"
		player,hero=@map.newPlayer(name)
		
		h=makeNewHeroMessage(player,hero)
		@server.sendToAllBut(h,c)

		d=Document.new
		d.root.setName("antargisLevel")
		@map.saveXML(d.root)
		puts "toxml"
		c=d.toString
		File.open("test.level","w").puts c
		#c="kasjdksdf skjsh skjsd sddsf  sdfkj sdfksd\nsdkjhdskjfh"
		m=WelcomeMessage.new(compress(c))
		puts "ok"
		return m
	end
	def syncCall(&block)
		#puts s
		puts "SYNC CALL"
		@queue.push(block)
		puts "ok"
	end
end


def startServer
	app=AntServer.new
	#app.disableGC
	app.run
end

startServer