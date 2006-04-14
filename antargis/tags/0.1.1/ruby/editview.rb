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

require 'ents.rb'

class AntRubyEditView<GLApp
	include AGHandler
	def initialize(w,h,map)
		super(w,h)
		$map=map
		@map=map
		@size=3
		$scene=getScene
	
		@layout=AGLayout.new(nil)
		@layout.loadXML(loadFile("data/gui/layout/edit_layout.xml"))
		
		puts self.methods.sort.join(" ")
		puts "------------------------"
		setMainWidget(@layout)
		@mainWidget=@layout
		addHandler(@layout.getChild("allWater"),:sigClick,:eventAllWater)
		addHandler(@layout.getChild("allGrass"),:sigClick,:eventAllGrass)
		
		addHandler(@layout.getChild("rubber"),:sigClick,:eventRubber)
		
		decos=["flower","gravel","grassLight","hole","rock","coach","floor","path","block"]
		decos.each{|name|
			#addHandler(@layout.getChild(name),:sigClick,:sigDeco)
		}
		
		decals=["gravel"]
		decals.each{|name|
			addHandler(@layout.getChild(name),:sigClick,:eventDecal)
		}
		
		
		ents=["sheep","hero","tower","druid","stones","farm","farmstead","workshop","fir","grassGreen","twig","man","tree","bush","highGrass"]
		ents.each{|name|
			addHandler(@layout.getChild(name),:sigClick,:eventAddEnt)
		}
		
		addHandler(@layout.getChild("pointer"),:sigClick,:eventPointer)
		
		[1,2,3,5,10,15].each{|s|addHandler(@layout.getChild("edit#{s}"),:sigClick,:eventSize)}
		["editHeight","editWater","editSand","editGround","editGrass","editGrass2","editForest","editRock"].each{|n|
			addHandler(@layout.getChild(n),:sigClick,:eventSelectEdit)
		}
		[1,2,3].each{|h|addHandler(@layout.getChild("hard#{h}"),:sigClick,:eventHard)}
		
		@hard=2
		
		addHandler(@layout.getChild("terrain"),:sigClick,:eventTabSelect)
		addHandler(@layout.getChild("entities"),:sigClick,:eventTabSelect)
		
		addHandler(@layout.getChild("new"),:sigClick,:eventNewMap)
		@buttonlayout=@layout
		setTab("terrain")
	end
	
	def eventNewMap
		#getMap.newMap(64,64)
		#getMap.setHeight(-0.5)
		if not @newDialog
			@layout.addChild(@newDialog=AGLayout.new(@layout))
			@newDialog.loadXML(loadFile("data/gui/layout/newdialog.xml"))
			addHandler(@newDialog.getChild("ok"),:sigClick,:eventNewMapEnd)
			addHandler(@newDialog.getChild("cancel"),:sigClick,:eventNewMapEnd)
			addHandler(toAGWindow(@newDialog.getChild("window")),:sigClose,:eventNewMapEnd)
		end
		return true
	end
	
	def eventNewMapEnd(e)
		name=e.getCaller.getName
		d=@newDialog
		@newDialog=nil
		if d
			@layout.removeChild(d)
			#d.close
			if name=="ok"
				w=toAGEdit(d.getChild("width")).getText.to_i
				h=toAGEdit(d.getChild("height")).getText.to_i
				
				if w and h
					w=2**(Math::log(w)/Math::log(2)).to_i
					h=2**(Math::log(h)/Math::log(2)).to_i
					if w>=16 and h>=16
						getMap.newMap(w,h)
						getMap.setHeight(-0.5)
					end
				end
			end
			@map.mapChanged
		end
		return true
	end
	
	def setTab(name)
		["terrain","entities"].each{|e|
			if e==name
				w=@buttonlayout.getChild(e+"Tab")
				if w
					w.show
				end
			else
				w=@buttonlayout.getChild(e+"Tab")
				if w
					w.hide
				end
			end
		}
	end
	
	def eventTabSelect(e)
		setTab(e.getCaller.getName)
		return true
	end
		
	def eventClick(list,button)
		puts "eventClick"
		puts "------------------"
		puts "list-size:"+(list.length.to_s)
		if list.length>0
			if @modifier
				puts @modifier
				send(@modifier,list,button)
			end
		end
		#super(list)
		return true
	end
	def editHeight(list,button)
	
		diff={1=>0.5, 2=>1, 3=>1.5}[@hard]
	
		list.each{|c|
			if isTerrain(c.node)
				puts "TERRAIN:"
				puts c.node
				puts @terrain
				x=c.pos.x.to_i
				y=c.pos.y.to_i
				middle=@map.get(x,y)
				if button==1
					middle+=diff
				elsif button==3 #right
					middle-=diff
				end
				for dx in (x-@size)..(x+@size)
					for dy in (y-@size)..(y+@size)
						if dx>=0 and dx<=@map.getW and dy>=0 and dy<=@map.getH
							d=Math::sqrt((dx-x)**2+(dy-y)**2)
							v=1-(d/(@size))
							v=[1,[0,v].max].min
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
	
	def editGrass(list,button)
		list.each{|c|
			if isTerrain(c.node)
				puts "TERRAIN:"
				puts c.node
				puts @terrain
				x=c.pos.x.to_i
				y=c.pos.y.to_i
				middle=@map.getGrass(x,y)
				if button==1
					middle+=0.1
				elsif button==3 #right
					middle-=0.1
				end
				for dx in (x-@size)..(x+@size)
					for dy in (y-@size)..(y+@size)
						if dx>=0 and dx<=@map.getW and dy>=0 and dy<=@map.getH
							d=Math::sqrt((dx-x)**2+(dy-y)**2)
							v=1-(d/(@size))
							v=[[0,v].max,1].min
							ov=@map.getGrass(dx,dy)
							cv=middle*v+ov*(1-v)
							@map.setGrass(dx,dy,cv)
						end
					end
				end
				@map.mapChanged
			end
		}
	end
	def editTerrain(list,button)
		tt={"editWater"=>WATER,"editSand"=>SAND,"editGround"=>EARTH,"editGrass"=>GRASS,"editGrass2"=>GRASS2,"editForest"=>FOREST,"editRock"=>ROCK}[@terrainType]
		h={1=>0.3,2=>0.6,3=>1}[@hard]
		list.each{|c|
			if isTerrain(c.node)
				x=c.pos.x.to_i
				y=c.pos.y.to_i
				for dx in (x-@size)..(x+@size)
					for dy in (y-@size)..(y+@size)
						if dx>=0 and dx<=@map.getW and dy>=0 and dy<=@map.getH
							d=Math::sqrt((dx-x)**2+(dy-y)**2)
							v=1-(d/(@size))
							puts v,v.class
							v=[[0,v].max,1].min
							dputs dx,dx.class,dy,dy.class,tt,tt.class
							ov=@map.getTerrain(dx,dy,tt)
							cv=[ov+v*h,1].min
							@map.setTerrain(dx,dy,tt,cv)
							printf "EDIT:%d,%d,%d,%f\n",dx,dy,tt,cv
						end
					end
				end
				@map.mapChanged
			end
		}
	end
	
	def eventSelectEdit(e)
		name=e.getCaller.getName
		if name=="editHeight"
			@modifier=e.getCaller.getName
		else
			@modifier="editTerrain"
			@terrainType=e.getCaller.getName
		end
		return true
	end
	
	def eventSize(e)
		@size=e.getCaller.getName[4..10].to_i
		return true
	end
	def eventHard(e)
		@hard=e.getCaller.getName[4..10].to_i
		return true
	end
	
	def eventPointer
		@modifier="edit"
		return true
	end
	
	def eventRubber
		@modifier="doRubber"
		return true
	end
	
	def eventAddEnt(e)
		callerName=e.getCaller.getName
		@modifier="addEntity"
		case callerName
			when "grassGreen"
				@type=AntGrass
			when "sheep"
				@type=AntNewSheep
			when "tower"
				@type=AntTower
			when "farm"
				@type=AntFarm
			when "fir"
				@type=AntFir
			when "workshop"
				@type=AntWorkshop
			when "farmstead"
				@type=AntFarmStead
			when "stones"
				@type=AntNewStone
			when "man"
				@type=AntNewMan
			when "hero"
				@type=AntHero
				@appearance="hero"
			when "druid"
				@type=AntNPC
				@appearance="druid"
			when "twig"
				@type=AntTwig
			when "tree"
				@type=AntNewTree
			when "bush"
				@type=AntBush
			when "highGrass"
				@type=AntHighGrass
		end
		return true
	end
	def eventDecal(e)
		callerName=e.getCaller.getName
		@modifier="addEntity"
		@type=AntDecal
		return true
	end
	def eventDeco(e)
		callerName=e.getCaller.getName
		@modifier="addEntity"
		if callerName=="coach"
			@type=AntDecoMesh
		else
			@type=AntNewDeco
		end
		@decoType=callerName
		return true
	end
	
	def isTerrain(node)
		[TerrainPieceVA,WaterPiece].member?(node.class)
	end
	
	def addEntity(list,button)
		pos=nil
		list.each{|c|
			if isTerrain(c.node)
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
		elsif @type==AntHero
			tree=@type.new
			tree.setAppearance(@appearance)
		else
			puts "TYPE:",@type
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
			if mesh.class==Mesh
				ent=getMap.getEntity(mesh)
				if ent
					getMap.removeEntity(ent)
				else
					puts "NOT FOUND!"
				end
				break
			end
		}
		getMap.move(0)
	end
	
	def eventAllWater
		getMap.setHeight(-0.5)
		return true
	end
	def eventAllGrass
		getMap.setHeight(1.0)
		return true
	end
	
	
end

