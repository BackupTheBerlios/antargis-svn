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

require 'ant_player.rb'
require 'ant_trigger.rb'
require 'ant_level.rb'
require 'entity.rb'

require 'ant_ai.rb'
require 'ant_path.rb'

require 'benchmark.rb'

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
# It might be important to mention, that currently only one map at a moment is supported, because
# AntRubyEntity contains a pointer the current instance - this makes it possible that all entities can
# access the map through getMap - FIXME
class AntRubyMap<AntMap
	attr_accessor :pause,:players
	attr_reader :path

	def initialize(app,pScene,w,h,playerName="Rowen")
# 		assert{app.is_a?(AGApplication)}
# 		assert{pScene.is_a?(SceneBase)}
		assert{w.is_a?(Numeric)}
		assert{h.is_a?(Numeric)}

		if pScene.nil?
			require 'ant_mock.rb'
		end

		super(pScene,w,h)
		@pause=false # is game paused
		@app=app

		@@systemTime=0.0  # systemTime is needed for the playing of sounds - so they won't be played too often
		@curTime=0.0      # curTime holds the current "date" of the world; the age of entities is measures by this

		@playerName=playerName
		@players=[]
		@myPlayer=nil

		@triggers=[]			# triggers are being touched by heroes and are checked in each frame

		@heroes=[]				# all heroes in the world

		@started=false		# started holds information about the map was already started. this is used for level-scripting

		@targets={}

		@filename="dummy"  # a dummy filename - used for level scripting
		@uidstart=0
	end

	def disableScript
		@script=nil
	end

	def AntRubyMap.getSystemTime
		@@systemTime
	end

	def getUniqueID
		@uidstart+=1
		@uidstart
	end
	def checkUID(i)
		@uidstart=[i,@uidstart].max
	end
	def getByUID(id)
		puts "WARNING: THIS function is slow!"
		ents=getAllEntities
		id=id.to_i
		ents.each{|e|
			r=e.get
			if r.uid==id
				return r
			end
		}
		puts id
		puts id.class
		raise "UID not found #{id}"
		nil
	end

	############################
	# event handlers
	############################

	# some delegators for level-scripting - this should eventually be done somehow else
	def eventHeroDied(ent)
		@heroes.delete(ent)
		if @script
			@script.eventHeroDied(AntLevelHero.new(ent))
		end
		@app and @app.setupHeroDisplay
	end
	def eventOwnerChanged(ent)
		if @script
			@script.eventOwnerChanged(AntLevelHero.new(ent))
		end
		@app and @app.setupHeroDisplay
	end
	def eventHLJobFinished(hero,job)
		if @script
			@script.eventHLJobFinished(AntLevelHero.new(hero),AntLevelJob.new(job))
		end
	end
	def eventHLDismissed(hero)
		if @script
			@script.eventDismissed(AntLevelHero.new(hero))
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
	
	def getHousesOfVillage(name)
		getAllEntities.collect{|e|e.get}.select{|e|e.is_a?(AntHouse)}.select{|e|e.village==name}
	end

	def getVillages
		getBuildings.collect{|e|e.village}.sort.uniq
	end

	def getHeroes
		getByType(AntHero)
	end
	def getBuildings
		getByType(AntHouse)
	end
	def getByType(type)
		getAllEntities.select{|e|e.is_a?(type)}
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

	# take heroes from AntPlayer object
	def getOwnHeroes()
		getPlayer.getHeroes if getPlayer
	end

	################################
	# loading & saving
	################################
	
	def processXMLNode(node)
		nodeName=node.getName
		nodeName.gsub!("New","")  # remove New out of old antNew.. Names

		return if nodeName.length<1

		entTypeMap=xmlName2ClassMap
		if entTypeMap.keys.member?(nodeName)
			e=entTypeMap[nodeName].new(self)
			@loadedEntsNum+=1
			if e.is_a?(AntHero)
				@heroes.push(e)
			end
		end
		if e
			e.preloadXML(node)
			@loadedEntities<<[node,e]
		end
# 	end
# 
# 	def loadEntityFromXML(e,node)
		if node.getName=="humanPlayer" then
			player=AntHumanPlayer.new(self,"")
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
		
		# FIXME: deprecated - remove this list!
		playerTypes={"computerPlayer"=>AntComputerPlayer, "lazyPlayer"=>AntLazyPlayer, "conqueringPlayer"=>AntConqueringPlayer,"newAI"=>AntAttackAI}

		playerTypes.update(getPlayerTypeMap)

		if playerTypes.keys.member?(node.getName) then
			type=playerTypes[node.getName]
			if type.ancestors.member?(AntPlayer)
				player=playerTypes[node.getName].new(self,"")
			else
				player=AntAIPlayer.new(node.get("name"),self)
				aiInterface=AIInterface.new(self,player)
				ai=type.new(aiInterface)
				player.ai=ai
			end
		
			player.loadXML(node)
			@players.push(player)
		end
		
		if node.getName=="trigger" then
			@triggers.push(Trigger.new(node))
		end
	end
	
	def loadXML(n)
		puts "loadXML(n)"
		@loadedEntsNum=1
		@loadedEntities=[]
		super(n)
		puts "loadXML(n) -1 "
		@loadedEntities.each{|pair|
			node,entity=pair
			insertEntity(entity)
		}

		@loadedEntities.each{|pair|
			node,entity=pair
			entity.loadXML(node)
			entity.eventMapChanged
		}
		
		createPathfinder

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
			pClass=eval(cl)
			if pClass.ancestors.member?(AntLevelScript)
				interface=AntLevelInterface.new(self,@app)
				@script=pClass.new(interface)
			end
	
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
			dputs name,name.class
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
		#time*=@app.getSpeed # increase speed  # moved to app
		@curTime+=time
		@@systemTime+=time
		super(time)
		
		checkTriggers
		
		@players.each{|player|
			player.move(time)
		}
		
		AntSound.ambientSound(time)
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
			if @script.eventTrigger(AntLevelHero.new(hero),t)
				done=true
			end
		end
		if not done
			@players.each{|p|p.trigger(hero,t)}
		end
	end

	def mapChanged
		super
		@app.setupNames
	end

	def getAllByType(type)
		getAllEntities.select{|e|
			e.is_a?(type)
		}
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


	def createPathfinder
		puts "createPathfinder"
		@path=CombinedPathFinder.new(self)
		@path.scene=getScene
		@path.displayPathfindingGraph(self,getScene)
		puts "createPathfinder ready"
	end

	# returns a map of possible xmlNames to their classes like {"antMan"=>AntMan}
	def xmlName2ClassMap
		return @classMapCache if @classMapCache

		# gather all entity types and map them to their xmlNames
		entTypes=getDescendantsOfClass(AntRubyEntity)
		entTypeMap={}
		entTypes.each{|t|
			entTypeMap[makeXmlName(t.to_s)]=t
		}
		@classMapCache=entTypeMap
	end

	def getPlayerTypeMap
		return @playerTypeMap if @playerTypeMap


		types=getDescendantsOfClass(AntBasicAI)
		typeMap={}
		types.each{|t|
			typeMap[makeXmlName(t.to_s)]=t
		}
		@playerTypeMap = typeMap
	end
	def makeXmlName(str)
		xml=str.to_s
		xml[0..0].downcase+xml[1..-1]
	end

end


