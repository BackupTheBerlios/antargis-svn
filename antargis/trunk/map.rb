#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# map.rb
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

require 'ant_player.rb'

class AntRubyMap<AntargisMap
	def initialize(w,h)
		super(w,h)
		@ents={}
		@players=[]
		GC.start
	end
	def loadEntity(node)
		puts "LOADENT"
		#GC.start
		puts "LOADENT"
		
		puts "name:"
		puts node.getName
		puts "---"
		if node.getName=="antTree" then
			e=AntNewTree.new
			puts "Raus"
			return e
		end
		
		e=super(node)
		if e then return e end
		if node.getName=="antNewMan" then
			e=AntNewMan.new
		end
		if node.getName=="antNewHero" then
			e=AntNewHero.new
		end
		if node.getName=="antNewHouse" then
			e=AntNewHouse.new
		end
		if node.getName=="antNewStone" then
			e=AntNewStone.new
		end
		if node.getName=="antNewTree" then
			e=AntNewTree.new
		end
		if node.getName=="antNewSheep" then
			e=AntNewSheep.new
		end
		if node.getName=="humanPlayer" then
			player=AntHumanPlayer.new("")
			player.loadXML(node)
			@players.push(player)
		end
		if node.getName=="computerPlayer" then
			player=AntComputerPlayer.new
			player.loadXML(node)
			@players.push(player)
		end
		puts "LOADENTITY:"
		puts e
		#GC.start
		if e then
			rubyID=e.getID.to_s
			e.setVar("RubyID",rubyID)
			@ents[rubyID]=e
			puts e.getType
			puts "getID:"+e.getID.to_s
		end
		return e
	end
	
	def insertEntity(e)
		GC.start
		puts "INSERTENTITY"
		puts e
		puts "type:"
		puts e.getType
		puts "getID:"+e.getID.to_s
		puts "rubyID..."
		rubyID=e.getVar("RubyID")
		puts "RUBYID"
		if rubyID=="" then
			rubyID=getNewID.to_s
			e.setVar("RubyID",rubyID)
		end
		if not @ents.key?(rubyID) then
			puts "IF"
			@ents[rubyID]=e
		else
			puts "ALREADY INSERTED:"+rubyID.to_s
		end
		super(e)
	end
	def removeEntity(e)
		@ents.delete(e.getID)
		super(e)
	end
	def clear
		@ents.clear
		super()
	end
	def getById(ent)
		@ents[ent.getVar("RubyID")]
	end
	def getRuby(ent)
		getById(ent)
	end
	def getEntity(id)
		getRuby(super(id))
	end
	def getByName(name)
		getRuby(super(name))
	end
	def endChange
	end
	
	def move(time)
		super(time)
		@players.each{|player|
			player.move(time)
		}
	end
	
	def saveXML(n)
		super(n)
		@players.each{|player|
			c=n.newChild(player.xmlName)
			player.saveXml(c)
		}
	end
	
	def getNext(ent,type)
		getRuby(super(ent,type))
	end
end


def getMap
	$map
end