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

class AntRubyEditView<EditIsoView
	include AGHandler
	def initialize(w,r,pos,map)
		super(w,r,pos,map)
		setEditing(true)
		setName("MYYYYEditView")
		
		@layout=AGLayout.new(self,loadFile("edit_layout.xml"))
		addChild(@layout)
		
		addHandler(@layout.getChild("allWater"),:sigClick,:sigAllWater)
		addHandler(@layout.getChild("grass"),:sigClick,:sigAllGrass)
		addHandler(@layout.getChild("rubber"),:sigClick,:sigRubber)
		
		addHandler(@layout.getChild("tree"),:sigClick,:sigTree)
		addHandler(@layout.getChild("deco"),:sigClick,:sigDeco)
		addHandler(@layout.getChild("coach"),:sigClick,:sigCoach)
		addHandler(@layout.getChild("stones"),:sigClick,:sigStones)
		addHandler(@layout.getChild("tower"),:sigClick,:sigTower)
		addHandler(@layout.getChild("hero"),:sigClick,:sigHero)
		
		addHandler(@layout.getChild("pointer"),:sigClick,:sigPointer)
		
		addHandler(@layout.getChild("pins"),:sigClick,:sigPoints)
		
		addHandler(@layout.getChild("edit1"),:sigClick,:sigEdit1)
		addHandler(@layout.getChild("edit2"),:sigClick,:sigEdit2)
		addHandler(@layout.getChild("edit3"),:sigClick,:sigEdit3)
		addHandler(@layout.getChild("edit4"),:sigClick,:sigEdit4)
		addHandler(@layout.getChild("edit5"),:sigClick,:sigEdit5)
		addHandler(@layout.getChild("edit10"),:sigClick,:sigEdit10)
		addHandler(@layout.getChild("edit15"),:sigClick,:sigEdit15)
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
	
	def sigPoints
		toggleShowPoints
	end
	
	def sigRubber
		@modifier="doRubber"
	end
	
	def sigTree
		@modifier="addEntity"
		@type=AntNewTree
	end
	def sigDeco
		@modifier="addEntity"
		@type=AntNewDeco
	end
	def sigCoach
		@modifier="addEntity"
		@type=AntNewCoach
	end
	def sigStones
		@modifier="addEntity"
		@type=AntNewStone
	end
	def sigTower
		@modifier="addEntity"
		@type=AntNewHouse
	end
	def sigHero
		@modifier="addEntity"
		@type=AntNewHero
	end
	
	def editHeight(ents)
		puts "EDITHEIGHT:"
		puts toInt($clickEvent.getButton)
		if toInt($clickEvent.getButton)==3
			dir=-1
		else
			dir=1
		end
		pos=getMarkerPos
		x=pos.x*2/TILE_WIDTH+2
		y=pos.z*2/TILE_WIDTH+3
		getMap.addFlat(x,y,15*dir,@size)
		getMap.addFlat(x,y,30*dir,@size-1)
	end
		
	def addEntity(ents)
		puts "ADDENTITY"
		pos=getMarkerPos
		tree=@type.new
		puts "tree:"
		puts tree
		addx=(rand()*32).to_i-16
		addz=(rand()*32).to_i-16
		tree.setPos2D(Pos2D.new(pos.x+addx,pos.z+addz))
		getMap.insertEntity(tree)
	end
	
	def edit(ents)
		ents.each{|e|
			e2=e.get
			ent=getMap.getRuby(e2)
			if ent
				editProperties(ent)
			end
		}
	end
	
	def editProperties(ent)
		d=AntEditPropDialog.new($screen,ent)
		$screen.addChild(d)
	end
	
	def doRubber(ents)
		puts "DOING RUBBER"
		ents.each{|e|
			e2=e.get
			ent=getMap.getRuby(e2)
			if ent==nil then ent=e2 end
			getMap.removeEntity(ent)
			break
		}
	end
	
	def sigAllWater
		getMap.setAllWater
	end
	def sigAllGrass
		getMap.setAllLand
	end
	
	def clickEntities(ents)
		puts "CLICKENTS"
		if @modifier
			puts "METHODS"
			send(@modifier,ents)
		end
	end
	
	def editMarkClicked(p,e)
		puts "EDITMARKCLICKED"
		if @modifier
			puts "METHODS"
			$clickEvent=e # dirty hack
			send(@modifier,[])
			$clickEvent=nil
		end
		
		#puts @modifier.methods.sort
	end
	
end

class AntRubyEditViewCreator<AGLayoutCreator
	def initialize()
		super("antRubyEditView")
	end
	def create(parent,rect,node)
		w=AntRubyEditView.new(parent,rect,Pos3D.new(0,0,0),$map)
		return w
	end
end
$antRubyEditViewCreator=AntRubyEditViewCreator.new

