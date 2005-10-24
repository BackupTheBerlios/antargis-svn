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

class AntRubyMap<AntMap
	def initialize(w,h)
		super(w,h)
		@ents={}
		@players=[]
		GC.start
		@lastGC=0
	end
	def loadEntity(node)
		case node.getName
			when "antFarm"
				e=AntFarm.new
			when "antWorkshop"
				e=AntWorkshop.new
			when "antFarmStead"
				e=AntFarmStead.new
			when "antFir"
				e=AntFir.new
			when "antBirch"
				e=AntBirch.new
		end
		if node.getName=="antNewMan" then
			e=AntNewMan.new
		end
		if node.getName=="antHero" then
			e=AntHero.new
		end
		if node.getName=="antTower" then
			e=AntTower.new
		end
		if node.getName=="antNewStone" then
			e=AntNewStone.new
		end
		if node.getName=="antNewTree" or node.getName=="antTree" then
			e=AntNewTree.new
		end
		if node.getName=="antNewSheep" then
			e=AntNewSheep.new
		end
		if node.getName=="antNewDeco" then
			e=AntNewDeco.new
		end
		if node.getName=="humanPlayer" then
			player=AntHumanPlayer.new("")
			player.loadXML(node)
			@players.push(player)
		end
		if node.getName=="computerPlayer" then
			player=AntComputerPlayer.new("")
			player.loadXML(node)
			@players.push(player)
		end
		return e
	end
	
	def removeEntity(e)
		@ents.delete(e.getID)
		super(e)
	end
	def clear
		@ents.clear
		super()
	end
	def getByIdOld(ent)
		@ents[ent.getVar("RubyID")]
	end
	def getById(ent)
		if ent
			return @ents[ent.getID]
		end
	end
	def getRuby(ent)
		getById(ent)
	end
	#def getEntity(id)
	#	getRuby(super(id))
	#end
	def pause
	end
	def unpause
	end
	def getByName(name)
		if name.class!=String
			puts name
		end
		getRuby(super(name))
	end
	def endChange
	end
	
	def move(time)
		super(time)
		@players.each{|player|
			player.move(time)
		}
		
		# disable GC, when called too often
		#if getGCcalls>3 then
		#	GC.disable
		#	@lastGC=0
		#end
		#resetGCcalls
		
		# enable GC after 2 secs, when it's called too often
		@lastGC+=time
		if @lastGC>2000 then
			GC.enable
		end
	end
	
	def saveXML(n)
		super(n)
		@players.each{|player|
			c=n.newChild(player.xmlName)
			player.saveXML(c)
		}
	end
	
	def getNext(ent,type)
		getRuby(super(ent,type))
	end
	
	def registerEntity(e)
		if not @ents.member?(e.getID)
			@ents[e.getID]=e
		end
	end
	
	def setLight(e)
	end
end


def getMap
	$map
end