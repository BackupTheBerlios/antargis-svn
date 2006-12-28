#--
# Copyright (c) 2006 by David Kamphausen. All rights reserved.
#
# ant_level.rb
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
#++
#
# == Level Scripting Overview
# The level-scripting is designed to be safe with respect to downloading a level
# and running it on your box. So in future releases it shall be put into a sandbox 
# (FFSandbox)
# 
# 

require 'ant_scripting.rb'

# This is the base scripting interface. Everything you want to do within level-scripting
# can be accessed through this class and the objects you can get here. 
class AntLevelInterface
	# you won't be able to create such an interface within a script. It will be created for you
	# in map.rb
	def initialize(map,app)
		@map=map
		@app=app
	end

	# get interface-objects for all players. returns an array of all the players
	def getPlayers
		@map.getPlayers.collect{|p|AntLevelPlayers.new(p)}
	end

	# use this to set the level's state to "won"
	def wonLevel
		@app.setResult(GameResult.new("won"))
	end
	# use this to set the level's state to "lost"
	def lostLevel
		@app.setResult(GameResult.new("lost"))
	end
	# calling this function will instantly end this level
	# please set the level's state first by calling wonLevel or lostLevel
	# before calling this function
	def endLevel
		@app.tryQuit
	end

	# tellStory will pause the game and display some text-dialogs in the middle of the screen
	# - flow is of type StoryFlow - go there for a examples on how to pack a story into this one
	def tellStory(flow)
		return unless flow.is_a?(StoryFlow)
		@app.tellStory(flow)
		@story=flow
	end

end

class AntLevelPlayers
	def initialize(player)
		@player=player
	end
	def getHeroes
		@player.getHeroes.select{|p|p.is_a?(AntHero)}.collect{|p|AIMyHero.new(p,@player)}
	end
	def getBuildings
		@player.getHeroes.select{|p|p.is_a?(AntHouse)}.collect{|p|AIMyBuilding.new(p,@player)}
	end
	def getName
		@player.getName
	end
end

class AntLevelEntity
	include AntScriptingEntityFullAccess
	def initialize(ent)
		@ent=ent
	end
	def is_a?(pclass)
		@ent.is_a?(pclass)
	end
end

class AntLevelHero<AntLevelEntity
	include AntScriptingHeroFullAccess
	def getPlayer
		AntLevelPlayers.new(@ent.getPlayer)
	end
end

class AntLevelJob
	def initialize(job)
		@job=job
	end
	def is_a?(pclass)
		@job.is_a?(pclass)
	end
	def target
		AntLevelEntity.new(@job.target)
	end
end

class AntLevelScript
	def initialize(interface)
		@interface=interface
		puts "INTERFACE:",@interface,self
	end
	def eventTrigger(hero,trigger)
	end
	def eventOwnerChanged(ent)
	end
	def eventHeroDied(ent)
	end
	def eventHLJobFinished(hero,job)
	end
	def eventDismissed(hero)
	end
	def eventLevelStarted
	end

	def saveXML(node)
		saveLocals(node)
	end
	def loadXML(node)
		loadLocals(node)
	end

# maybe exchange this
private
	def getLocals
		a={}
		instance_variables.each{|v|
			a[v]=instance_variable_get(v)
		}
		return a
	end
	def saveLocals(node)
		getLocals.each{|n,v|
			c=node.addChild("local")
			if not v.is_a?(AntLevelInterface)
				c.set("name",n)
				c.set("type",v.class.to_s)
				c.set("value",v.to_s)
			end
		}
	end
	def loadLocals(node)
		node.getChildren("local").each{|c|
			n=c.get("name")
			v=nil
			case c.get("type")
				when "TrueClass"
					v=true
				when "FalseClass"
					v=false
				when "Fixnum","Bignum"
					v=c.get("value").to_i
				when "Float"
					v=c.get("value").to_f
				when "String"
					v=c.get("value")
			end
			if v
				instance_variable_set(n,v)
			end
		}
	end

end

