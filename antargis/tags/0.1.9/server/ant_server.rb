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


# add ruby and server directory to PATH
$programDir=Dir.pwd+"/ruby"
$:.push($programDir)
$serverDir=Dir.pwd+"/server"
$:.push($serverDir)

require 'antargislib.rb'
require 'server/config.rb'
require 'server.rb'
require 'ents.rb'
require 'map.rb'

#
# AntServer is done from scratch, because we don't want any graphics display here.
# it contains:
#   - a Map-object (the game-world)
#   - a simple gui for controlling the world:
#     - watching the chat
#     - kicking players
#     - quitting
# FIXME:
#  - provide support for no display whatsoever (server with X)

class AntServer<AGApplication
	def initialize
		puts "agApp::"
		super
		puts "agApp!"
		# init GUI
		#@layout=AGLayout.new(nil)
		#@layout.loadXML(loadFile("data/gui/layout/server.xml"))
		#setMainWidget(@layout)

		# add virtual Scene 
		# FIXME: this should be discarded)
		@scene=Scene.new(128,128)
		# add Map
		@map=AntRubyMap.new(@scene,128,128)
		$map=@map
		# load some map - currently a default one
		@map.loadMap("levels/multiplayer/multimap1.antlvl")

		# generate some login-table and insert some users
		table=SimpleLoginTable.new
		table.addPlaintext("muh","puh")
		# now start the server-object
		@server=LoginServer.new(table,self)
		@queue=Queue.new
	end
	def eventFrame(t)
		delay(20) # server should run at relatively low FPS=20 - this should be sufficient and saves CPU power
		@map.move(t)

		# do sync-calls
		while @queue.length>0
			puts "CALL BLOCK"
			b=@queue.pop
			b.call
		end

	end
	def eventNewPlayer(name,connection)
		puts "eventNewPlayer(name,connection)"
		player,hero=@map.newPlayer(name)
		
		#connection=nil # FIXME

		sendNewHeroMessages(player,hero,connection)
		#@server.sendToAllBut(h,connection)

		# make a savegame-text and send it to the new player
		doc=Document.new
		doc.root.setName("antargisLevel")
		@map.saveXML(doc.root)
		m=WelcomeMessage.new(compress(doc.toString))
		connection.sendMessage(m)
	end

	#private
	# this function executes any blocks that were queued in other threads in the main-thread
	def syncCall(&block)
		puts "PUSH BLOCK"
		@queue.push(block)
	end
	private
	def sendNewHeroMessages(player,hero,connection)
		@server.sendToAllBut(NewPlayerMessage.new(player.getName,hero.getPos2D),connection)
	end
end


# start some dummy server
def startServer
	app=AntServer.new
	app.run
end

startServer

