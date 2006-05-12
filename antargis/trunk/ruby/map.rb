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

def getDescendantsOfClass(p)
	c=[]
	ObjectSpace.each_object(Class){|a|c.push(a) if a.ancestors.member?(p)}
	return c
end

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

class AntRubyMap<AntMap
	def initialize(pScene,w,h)
		super(pScene,w,h)
		@ents={}
		@players=[]
		#GC.start
		@lastGC=0
		$systemTime=0.0
		@myPlayer=nil
		@triggers=[]
		@heroes=[]
		@started=false
		@story={}
		@targets={}
		@curTime=0.0

		# gather all entity types and map them to their xmlNames
		@entTypes=getDescendantsOfClass(AntRubyEntity)
		@entTypeMap={}
		@entTypes.each{|t|
			xml=t.to_s
			xml=xml[0..0].downcase+xml[1..1000]
			@entTypeMap[xml]=t
		}

	end
	def getTarget(name)
		@targets[name]
	end
	def getPlayer
		@myPlayer
	end
	def loadEntity(node)
		dputs node.getName

		nodeName=node.getName
		nodeName.gsub!("New","")

		if @entTypeMap.keys.member?(nodeName)
			e=@entTypeMap[nodeName].new
		end
		if node.getName=="humanPlayer" then
			dputs "1"
			player=AntHumanPlayer.new("")
			dputs "2"
			player.loadXML(node)
			dputs "3"
			@players.push(player)
			dputs "4"
			if not @myPlayer
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
		if node.getName=="story" then
			loadStory(node)
		end
		return e
	end
	
	def loadStory(node)
		node.getChildren.each{|n|
			name=n.getName
			a=[]
			n.getChildren.each{|n2|
				if n2.getName=="window"
					text=""
					n2.getChildren("text").each{|n3|
						text+=n3.getText
					}
					a.push(text)
				end
			}
			@story[name]=a
		}
	end

	def loadMap(filename)
		@filename=filename
		super
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
 		if ent
 			return @ents[ent.getID]
 		end
 	end
	def pause
		@paused=true
	end
	def unpause
		@paused=false
	end
	def paused
		@paused
	end
	def getByName(name)
		if name.class!=String
			dputs name
		end
		super(name)
	end
	def endChange
	end
	
	def checkTriggers
		@heroes.each{|h|
			@triggers.each{|t|
				t.check(h)
			}
		}
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
			#if @story[t.name]
			#	playStory(t.name)
			#end
		end
	end
	def getTime
		@curTime
	end
	
	def move(time)
		if @paused
			return
		end
		@curTime+=time
		$systemTime+=time
		super(time)
		
		checkTriggers
		
		@players.each{|player|
			player.move(time)
		}
		
		ambientSound(time)
		if not @started
		#	playStory("start")
			@started=true
			if @script
				@script.eventLevelStarted
			end
		end
	end
	
	def loadXML(n)
		super(n)
		@players.each{|p|p.move(0)}
		
		if n.get("scriptfile").length>0 and n.get("scriptclass").length>0
			@scriptFile=n.get("scriptfile")
			@scriptClass=n.get("scriptclass")
			c=loadFile(n.get("scriptfile"))
			levelName=getLevelName
			c="module #{levelName}\n"+c+"\nend\n"
			puts c
			puts eval(c)
			cl="#{levelName}::"+n.get("scriptclass")
			@script=eval(cl).new
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

	def getLevelName
		"L_"+@filename.gsub(".rb","").gsub(".","_").gsub("/","_")
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
	
	def getNext(ent,type)
		super(ent,type)
	end
	
	def registerEntity(e)
		if not @ents.member?(e.getID)
			@ents[e.getID]=e
		end
	end
	
	def setLight(e)
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

	def getPortrait(hero)
		f="data/gui/portraits/#{hero}.png"
		if fileExists(f)
			return AGSurface.load(f)
		else
			return AGSurface.load("data/gui/portraits/none.png")
		end
	end
end


def getMap
	$map
end
