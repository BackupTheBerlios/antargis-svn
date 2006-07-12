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
require 'ant_trigger.rb'
require 'level.rb'

# This class prodives support for defining target-positions in a level-file.
# These positions can be used for scripting. This way code and level-data is
# devided (MVC)
class TargetPos
	attr_reader :pos, :name
	def loadXML(n)
		@pos=AGVector2.new(n.get("x").to_f,n.get("y").to_f)
		@name=n.get("name")
	end
	def saveXML(n)
		n.set("x",@pos.x.to_s)
		n.set("y",@pos.y.to_s)
		n.set("name",@name)
	end
	def xmlName
		"target"
	end
end


# AntRubyMap is not only the "map", but manages the moving and the actions of all the
# entities around. Apart from that it contains the Players. So it might be better to call it "World"
class AntRubyMap<AntMap
	attr_accessor :pause,:players

	def initialize(pScene,w,h,playerName="Rowen")
		super(pScene,w,h)
		@pause=false # is game paused

		$systemTime=0.0  # systemTime is needed for the playing of sounds - so they won't be played too often
		@curTime=0.0     # curTime holds the current "date" of the world; the age of entities is measures by this

		@playerName=playerName
		@players=[]
		@myPlayer=nil

		@triggers=[]			# triggers are being touched by heroes and are checked in each frame

		@heroes=[]				# all heroes in the world

		@started=false		# started holds information about the map was already started. this is used for level-scripting

		@targets={}

		# gather all entity types and map them to their xmlNames
		@entTypes=getDescendantsOfClass(AntRubyEntity)
		@entTypeMap={}
		@entTypes.each{|t|
			xml=t.to_s
			xml=xml[0..0].downcase+xml[1..1000]
			@entTypeMap[xml]=t
		}

		@filename="dummy"  # a dummy filename - used for level scripting
	end

	############################
	# event handlers
	############################

	# some delegators for level-scripting - this should eventually be done somehow else
	def eventHeroDied(ent)
		@heroes.delete(ent)
		if @script
			@script.eventHeroDied(ent)
		end
		$app.setupHeroDisplay
	end
	def eventOwnerChanged(ent)
		if @script
			@script.eventOwnerChanged(ent)
		end
		$app.setupHeroDisplay
	end
	def eventHLJobFinished(hero,job)
		if @script
			@script.eventHLJobFinished(hero,job)
		end
	end
	def eventHLDismissed(hero)
		if @script
			@script.eventDismissed(hero)
		end
	end

	################################
	# getting information
	################################

	# get target-position which is stored unter "name" in the level-file
	def getTarget(name)
		@targets[name]
	end
	# get my player (the player-object of the player playing "this" client
	def getPlayer
		if @myPlayer.nil?
			raise "no player defined in level!"
		end
		@myPlayer
	end
	# get current world time
	def getTime
		@curTime
	end

	def getHeroes()
		ents=getAllEntitiesV
		ret=[]
		ents.each{|eptr|
			ent=eptr.get
			#dputs ent
			if ent.class==AntHero
				ret.push(ent)
			end
		}
		return ret
	end

	# returns AGSurfaces of the given hero for portraits
	def getPortrait(hero)
		f="data/gui/portraits/#{hero}.png"
		if fileExists(f)
			return AGSurface.load(f)
		else
			return AGSurface.load("data/gui/portraits/none.png")
		end
	end

	# FIXME:rewrite this!!!!
	# take heroes from AntPlayer object
	def getOwnHeroes()
		ents=getAllEntitiesV
		ret=[]
		ents.each{|eptr|
			ent=eptr.get
			#dputs ent
			if ent.class==AntHero
				p=ent.getPlayer
				#dputs "player:",p,p.class
				if p
					if p.class==AntHumanPlayer
						ret.push(ent)
					end
				end
			end
		}
		return ret
	end

	################################
	# loading & saving
	################################
	
	def loadEntity(node)
		nodeName=node.getName
		nodeName.gsub!("New","")  # remove New out of old antNew.. Names

		if @entTypeMap.keys.member?(nodeName)
			e=@entTypeMap[nodeName].new

			if e.is_a?(AntHero)
				@heroes.push(e)
			end

		end
		if node.getName=="humanPlayer" then
			player=AntHumanPlayer.new("")
			player.loadXML(node)
			@players.push(player)
			if not @myPlayer
				@myPlayer=player
			elsif player.getName==@playerName
				@myPlayer=player
			end
		end
		if node.getName=="target" then
			t=TargetPos.new
			t.loadXML(node)
			@targets[t.name]=t
		end
		
		playerTypes={"computerPlayer"=>AntComputerPlayer, "lazyPlayer"=>AntLazyPlayer, "conqueringPlayer"=>AntConqueringPlayer}
		if playerTypes.keys.member?(node.getName) then
			player=playerTypes[node.getName].new("")
			player.loadXML(node)
			@players.push(player)
		end
		
		if node.getName=="trigger" then
			@triggers.push(Trigger.new(node))
		end
		return e
	end
	
	def loadXML(n)
		super(n)
		@players.each{|p|p.move(0)}
		
		if n.get("scriptfile").length>0 and n.get("scriptclass").length>0
	
			# FIXME: add some safetly level here!!!

			@scriptFile=n.get("scriptfile")
			@scriptClass=n.get("scriptclass")
			c=loadFile(n.get("scriptfile"))
			@filename=@scriptFile.gsub(".rb",".antlvl")
			levelName=getLevelName
			c="module #{levelName}\n"+c+"\nend\n"
			eval(c)
			cl="#{levelName}::"+n.get("scriptclass")
			@script=eval(cl).new
	
			puts @script.class
			#raise 1
	
		end
		if @script
			sd=n.getChildren("scriptdata")
			sd.each{|c|
				@script.loadXML(c)
			}
		end
		if n.get("curTime")!=""
			@curTime=n.get("curTime").to_f
		end
	end

	def loadMap(filename)
		@filename=filename
		super
	end

	def saveXML(n)
		super(n)
		@players.each{|player|
			c=n.addChild(player.xmlName)
			player.saveXML(c)
		}
		@targets.each{|name,t|
			c=n.addChild(t.xmlName)
			t.saveXML(c)
		}
		@triggers.each{|t|
			c=n.addChild(t.xmlName)
			t.saveXML(c)
		}
		if @scriptClass
			n.set("scriptclass",@scriptClass)
		end
		if @scriptFile
			n.set("scriptfile",@scriptFile)
		end
		if @script
			c=n.addChild("scriptdata")
			@script.saveXML(c)
		end
	end

	######################################
	# modify the world
	######################################
	
	# make a new player at pre-defined spawn-positions
	# used positions shall not be used again, until player leaves
	# returns a pair of player and hero objects.
	def newPlayer(name)
		# FIXME:get a free spawn point
		#
		@targets.each_key{|key|
			if key=~/spawn/
			end
		}
		pos=AGVector2.new(20,20)
		player=AntHumanPlayer.new(name)
		@players.push(player)
		#new hero
		hero=AntHero.new
		hero.setName(name)
		hero.setPlayer(player)
		hero.setPos(pos)
		insertEntity(hero)
		return player,hero
	end

	
	######################################
	# modify the world
	######################################

	def getByName(name)
		if name.class!=String
			dputs name
		end
		super(name)
	end
	def endChange
		mapChanged
	end

	# this function will be used for displaying lights and including "fog of war"
	def setLight(e)
	end

	def move(time)
		if @pause
			return
		end
		#time*=2 # increase speed
		@curTime+=time
		$systemTime+=time
		super(time)
		
		checkTriggers
		
		@players.each{|player|
			player.move(time)
		}
		
		ambientSound(time)
		if not @started
			@started=true
			if @script
				@script.eventLevelStarted
			end
		end
	end

	def trigger(hero,t)
		done=false
		if @script
			if @script.eventTrigger(hero,t)
				done=true
			end
		end
		if not done
			@players.each{|p|p.trigger(hero,t)}
		end
	end

	def mapChanged
		super
		$app.setupNames
	end

private	
	def checkTriggers
		@heroes.each{|h|
			@triggers.each{|t|
				t.check(h)
			}
		}
	end

	def getLevelName
		"L_"+@filename.gsub(".rb","").gsub(".","_").gsub("/","_")
	end
end


def getMap
	$map
end
