#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# view.rb
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

require 'ant_inventory.rb'
require 'ant_energy.rb'

class AntRubyView <GLApp #AGWidget #CompleteIsoView
	def initialize(w,h) #p,rect,pos,map)
		super(w,h) #p,rect,pos,map)
		$antView=self
		@mousepos=AGVector2.new(200,200)
	end
	

	def draw
		updateNamePositions
		super
	end
	def updateNamePositions
		#return
		heroes=$map.getHeroes
		heroes.each{|hero|
			@names.each{|name|
				if hero.getName==name.getText
					name.setRect(getHeroScreenPos(hero))
				end
			}
		}
	end
	
	def getHeroScreenPos(hero)
		pos=hero.getPos3D+AGVector3.new(0,0,2)
		sp=getScene.getPosition(AGVector4.new(pos,1))
		return AGRect.new(sp.x-50,sp.y-55,100,50)
	end

	def focusHero(hero)
		setCamera(hero.getPos2D)
		#getScene.setCamera(clipCamera(AGVector4.new(p.x,p.y,0)))
	end
	
	def setupNames
		if @names==nil
			heroes=$map.getHeroes
			@names=[]
			heroes.each{|hero|
				name=hero.getName
				@names.push(AntNameDisplay.new(@layout,getHeroScreenPos(hero),hero))
			}
			@names.each{|n|
				@layout.addChild(n)
			}
		end
	end

	def eventHover(list,button)
		if list.length>0
			n=list[0]
			mesh=n.node
			ok=false
			if mesh.class==Mesh
				ent=getMap.getEntity(mesh)
				if ent
					if ent.is_a?(AntBoss) then
						hoverEntity(ent)
						ok=true
					end
				end
			end
			if not ok
				hoverEntity(nil)
			end
		end
		return true
	end
	
	def eventClick(list,button)
# 		list.each{|l|
# 			puts l.node.to_s+"\t"+l.node.class.to_s+"  "+l.camDist.to_s
# 			if l.node.class==Mesh
# 				puts getMap.getEntity(l.node)
# 			end
# 		}
# 		puts "---"
		
		if list.length>0
			first=list[0]
			if [TerrainPieceVA,WaterPiece].member?(first.node.class) # == TerrainPieceVA or first.ntoTerrainMesh(first.node)==getMap.getTerrainMesh
				clickMap(first.pos,button)
			else
				clickEntities(list,button)
			end
		end
		return true
	end
	
	def hoverEntity(e)
		if @hover
			if @hover.class==AntHero or @hover.is_a?(AntHouse)
				@hover.hovered=false
			end
		end
		@hover=e
		if @hover
			if @hover.class==AntHero or @hover.is_a?(AntHouse)
				@hover.hovered=true
			end
		end
	end
	
	def setCamera(p)
		super(clipCamera(p))
	end

	def clipCamera(p)
		AGVector2.new([31,p[0],getMap.getW-31].sort[1],[14,p[1],getMap.getH-36].sort[1])
	end
	def eventMouseMotion(e)
		super
		@mousepos=e.getMousePosition
		return true
	end
	def checkMove(t)
		if getMain.fullscreen
			p=@mousepos
			m=0
			w=20
			s=10
			{AGRect.new(0,m,w,getMain.height-m*2)=>AGVector2.new(-1,0),
				AGRect.new(m,0,getMain.width-m*2,w)=>AGVector2.new(0,1),
				AGRect.new(getMain.width-w-1,m,w,getMain.height-m*2)=>AGVector2.new(1,0),
				AGRect.new(m,getMain.height-w-1,getMain.width-m*2,w)=>AGVector2.new(0,-1)}.each{|r,n|
				if r.contains(p)
					setCamera(getCamera+n*t*s) # FIXME: Include time
				end
			}
		end
	end
	def eventFrame(t)
		super
		checkMove(t)
		return true
	end
end

class AntRubyEditViewTest<AGWidget #EditIsoView
	def editMarkClicked(p,e)
		
	end
end


class AntRubyViewCreator<AGLayoutCreator
	def initialize()
		super("antRubyView")
	end
	def create(parent,rect,node)
		w=AntRubyView.new(parent,rect,AGVector3.new(0,0,0),$map)
		return w
	end
end
$antRubyViewCreator=AntRubyViewCreator.new




class AntButtonPanel<AGWidget
	attr_reader :job
	def initialize(p,r)
		super(p,r)
		setName("ButtonPanel")
		$buttonPanel=self
		clearHandlers
		#@jobButtons=["doPoint","doMove","doFight","doRecruit","doDismiss","doInvent"]
		@jobButtons=["doFight","doInvent","doRest","doDismiss","doDropFood","doDropWeapon"]
		@aggButtons={"doAgg0"=>1,"doAgg1"=>2,"doAgg2"=>3}
		@inited=false
		@agg=1

		addSignal("sigAggressionChanged")
		addSignal("sigJobChanged")
	end
	def initHandlers
		getChild("doAgg0").setChecked(true)
		getChild("doRest").setChecked(true)
		@job="doRest"
		@jobButtons.each {|b|
			c=getChild(b)
			addHandler(c,:sigClick,:eventJobSelected)
		}
		@aggButtons.each {|b,a|
			c=getChild(b)
			addHandler(c,:sigClick,:eventAggSelected)
		}
	end
	def draw(p)
		super(p)
	end

	def setName(n)
		if getChild("heroName")
			getChild("heroName").setText(n)
		end
	end
	
	def eventJobSelected(e)
		@job=e.getCaller.getName
		sigJobChanged(e)
		return true
	end
	def eventAggSelected(e)
		@agg=@aggButtons[e.getCaller.getName]
		sigAggressionChanged(e)
		return true
	end
	
	def setAggression(l)
		l=l.to_i
		@aggButtons.each{|n,b|
			getChild(n).setChecked((b==l))
		}
		@agg=l
	end
	def getAggression()
		return @agg
	end
	def setPointing
		@job="doPoint"
		getChild("doFight").setChecked(true)
	end
end

class AntButtonPanelCreator<AGLayoutCreator
	def initialize
		super("antButtonPanel")
	
	end
	def create(p,r,n)
		return AntButtonPanel.new(p,r)
	end
end
$antBPCreator=AntButtonPanelCreator.new


class AntNameDisplay<AGWidget
	def initialize(p,r,hero)
		super(p,r)
		@hero=hero
		@cr=r.origin
		@cr.setHeight(@cr.height/2)
		@cr.setTop(@cr.height)
		@cr=@cr.shrink(4)
		@font=getTheme.getFont("heroName.font")
		@oldfont=@font
		@name=@hero.getName
		addChild(@mb=AGButton.new(self,AGRect.new(0,0,width,height/2+4	),""))
		@mb.setEnabled(false)
		addChild(@textWidget=AGText.new(self,AGRect.new(0,0,width,height/2),@hero.getName,@font))
		
		#b.setTheme("antButton")
		if false
			addChild(b=AGButton.new(self,AGRect.new(0,height/2,width,height/2-1),""))
			b.setEnabled(false)
			@energyWidget=AntEnergy.new(self,AGRect.new(5,height/2+5,width-10,height/2-11))
			addChild(@energyWidget)
			@energyWidget.setHero(hero)
		end
		setCaching(true)
	end

	# enforce integer position	
	def setRect(r)
		super(AGRect.new(r.x.to_i,r.y.to_i,r.w.to_i,r.h.to_i))
	end
	
	def getText
		@name
	end

	def prepareDraw
		if @hero.getPlayer==$map.getPlayer
			@font=getTheme.getFont("heroName.font")
		else
			@font=getTheme.getFont("enemyHero.font")
		end
		if @font!=@oldfont
			@textWidget.setFont(@font)
			queryRedraw
		end
		super
	end
	private
end