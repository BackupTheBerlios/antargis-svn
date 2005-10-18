#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# editview.rb
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

#require 'libantargis'
#include Libantargis

class AntRubyEditView<GLApp
	include AGHandler
	def initialize(w,h,map)
		super(w,h)
		$map=map
		@map=map
		@size=3
		@terrain=TerrainMesh.new(@map)
		setupLight
		@map.setScene(getScene)
		$scene=getScene
		getScene.addNode(@terrain)
	
		@layout=AGLayout.new(nil,loadFile("data/gui/layout/edit_layout.xml"))
		setMainWidget(@layout)
		@mainWidget=@layout
		addHandler(@layout.getChild("allWater"),:sigClick,:sigAllWater)
		addHandler(@layout.getChild("grass"),:sigClick,:sigAllGrass)
		addHandler(@layout.getChild("rubber"),:sigClick,:sigRubber)
		
		addHandler(@layout.getChild("tree"),:sigClick,:sigTree)
		
		decos=["flower","gravel","grassLight","grassGreen","twig","hole","rock","coach","floor","path","block"]
		decos.each{|name|
			addHandler(@layout.getChild(name),:sigClick,:sigDeco)
		}
		
		ents=["sheep","hero","tower","druid","stones"]
		ents.each{|name|
			addHandler(@layout.getChild(name),:sigClick,:sigAddEnt)
		}
		
		addHandler(@layout.getChild("pointer"),:sigClick,:sigPointer)
		
		addHandler(@layout.getChild("edit1"),:sigClick,:sigEdit1)
		addHandler(@layout.getChild("edit2"),:sigClick,:sigEdit2)
		addHandler(@layout.getChild("edit3"),:sigClick,:sigEdit3)
		addHandler(@layout.getChild("edit4"),:sigClick,:sigEdit4)
		addHandler(@layout.getChild("edit5"),:sigClick,:sigEdit5)
		addHandler(@layout.getChild("edit10"),:sigClick,:sigEdit10)
		addHandler(@layout.getChild("edit15"),:sigClick,:sigEdit15)
	end
		
	def eventClick(list,button)
		puts "------------------"
		if list.length>0
			if @modifier
				puts @modifier
				send(@modifier,list,button)
			end
		end
		#super(list)
		return
	end
	def editHeight(list,button)
		list.each{|c|
			if toTerrainMesh(c.node)==@terrain
				puts "TERRAIN:"
				puts c.node
				puts @terrain
				x=c.pos.x.to_i
				y=c.pos.y.to_i
				middle=@map.get(x,y)
				if button==1
					middle+=1
				elsif button==3 #right
					middle-=1
				end
				for dx in (x-@size)..(x+@size)
					for dy in (y-@size)..(y+@size)
						if dx>=0 and dx<=@map.getW and dy>=0 and dy<=@map.getH
							d=Math::sqrt((dx-x)**2+(dy-y)**2)
							v=1-(d/(@size))
							v=[0,v].max
							ov=@map.get(dx,dy)
							cv=middle*v+ov*(1-v)
							@map.set(dx,dy,cv)
						end
					end
				end
				@map.mapChanged
			end
		}
	end
	
	def sigEdit1
		@modifier="editHeight"
		@size=1
	end
	def sigEdit2
		@modifier="editHeight"
		@size=2
	end
	def sigEdit3
		@modifier="editHeight"
		@size=3
	end
	def sigEdit4
		@modifier="editHeight"
		@size=4
	end
	def sigEdit5
		@modifier="editHeight"
		@size=5
	end
	def sigEdit10
		@modifier="editHeight"
		@size=10
	end
	def sigEdit15
		@modifier="editHeight"
		@size=15
	end
	
	def sigPointer
		@modifier="edit"
	end
	
	def sigRubber
		@modifier="doRubber"
	end
	
	def sigTree
		@modifier="addEntity"
		@type=AntNewTree
	end
	def sigAddEnt(name,callerName,event,caller)
		@modifier="addEntity"
		case callerName
			when "sheep"
				@type=AntNewSheep
			when "tower"
				@type=AntNewHouse
			when "stones"
				@type=AntNewStone
			when "hero"
				@type=AntNewHero
				@appearance="hero"
			when "druid"
				@type=AntNewHero
				@appearance="druid"
		end
	end
	def sigDeco(name,callerName,event,caller)
		@modifier="addEntity"
		@type=AntNewDeco
		@decoType=callerName
		return
	end
	
	def addEntity(list,button)
		pos=nil
		list.each{|c|
			if toTerrainMesh(c.node)==@terrain
				pos=c.pos
			end
		}
		if not pos
			return
		end
		#if toInt($clickEvent.getButton)!=1
		#	return
		#end
		puts "ADDENTITY"
		#pos=getMarkerPos
		dorand=true
		if @type==AntNewDeco
			tree=@type.new(@decoType)
			if @decoType=="floor" or @decoType=="block"
				dorand=false
			end
		elsif @type==AntNewHero
			tree=@type.new
			tree.setAppearance(@appearance)
		else
			tree=@type.new
		end
		puts "type:"
		puts tree
		tree.setPos(AGVector2.new(pos.x,pos.y))
		getMap.insertEntity(tree)
	end
	
	def edit(meshes,button)
		meshes.each{|res|
			node=res.node
			puts "NODE:"
			puts node
			if node.class==Mesh
			ent=getMap.getEntity(toMesh(node))
			if ent
				editProperties(ent)
			end
			end
		}
	end
	
	def getMarkerPos
		if @markerPos
			return @markerPos
		else
			return super()
		end
	end
	
	def editProperties(ent)
		d=AntEditPropDialog.new($screen,ent)
		$screen.addChild(d)
	end
	
	def doRubber(list,button)
		puts "DOING RUBBER"
		list.each{|res|
			mesh=res.node
			ent=getMap.getEntity(toMesh(mesh))
			if ent
				getMap.removeEntity(ent)
			end
			break
		}
	end
	
	def sigAllWater
		getMap.setHeight(-0.5)
		#getMap.setAllWater
	end
	def sigAllGrass
		getMap.setHeight(1.0)
		#getMap.setAllLand
	end
	
	
end

