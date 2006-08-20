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

class AntRubyView <GLApp
	def initialize(w,h)
		super(w,h)
		$antView=self
		@mousepos=AGVector2.new(200,200)
		@controls=true
	end

	def draw
		updateNamePositions
		super
	end
	def updateNamePositions
		heroes=$map.getHeroes
		@names.each{|k,w|
			found=false
			heroes.each{|hero|
				if hero.getName==w.getText
					w.setRect(getHeroScreenPos(hero))
					found=true
				end
			}
			if (not found)
				w.hide
			end
		}
	end
	
	def getHeroScreenPos(hero)
		pos=hero.getPos3D+AGVector3.new(0,0,2)
		sp=getScene.getPosition(AGVector4.new(pos,1))
		return AGRect.new(sp.x-50,sp.y-55,100,27)
	end

	def focusHero(hero)
		setCamera(hero.getPos2D)
	end
	
	def setupNames
		puts "SETUPNAMES"

		heroes=$map.getHeroes
		@names||={}
		heroes.each{|hero|
			name=hero.getName
			#FIXME: readd this
			if @names[hero].nil?
				n=AntNameDisplay.new(@layout,getHeroScreenPos(hero),hero)
				@names[hero]=n
				@layout.addChild(n)
			end
		}
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
		if (not @controls)
			puts "CONTROLS DISABLED"
			return false
		end
		if list.length>0
			first=list[0]
			if [TerrainPieceVA,WaterPiece].member?(first.node.class)
				eventMapClicked(first.pos,button)
			else
				eventEntitiesClicked(list,button)
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
		x=super(clipCamera(p))
		updateSoundPos
		x
	end
	def getCameraHeight(p)
		# take some middle
		h=0
		(-1..1).each{|x|
			(-1..1).each{|y|
				h+=getMap.getHeight(p[0]+x,p[1]+y)
			}
		}
		h/=9
		h=[0,h].max
		puts h
		return h
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
		@jobButtons=["doRest","doDismiss","doDropFood","doDropWeapon","doBuild"]
		@aggButtons={"doAgg0"=>1,"doAgg1"=>2,"doAgg2"=>3}
		@inited=false
		@agg=1

		addSignal("sigAggressionChanged")
		addSignal("sigJobChanged")

		@hero=nil
	end
	def setHero(h)
		@hero=h
	end
	def initHandlers
		getChild("doAgg0").setChecked(true)
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
	def prepareDraw
		updateJobView
		super
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
private
	def updateJobView
		return if @hero.nil?
		if @hero.getJob
			i=@hero.getJob.image
		else
			i="data/gui/bed.png"
		end
		if @job!=i
			@job=i
			getChild("jobView").setTexture(getTextureCache.get(i))
		end
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
		@font=getTheme.getFont("heroName.font")
		@oldfont=@font
		@name=@hero.getName
		addChild(@mb=AGButton.new(self,AGRect.new(0,0,width,height),""))
		
		@mb.setEnabled(false)
		addChild(@textWidget=AGText.new(self,AGRect.new(0,0,width,height),@hero.getName,@font))
		
		@fonts={true=>getTheme.getFont("heroName.font"),false=>getTheme.getFont("enemyHero.font")}
		@oldPlayer=nil
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
		@font=@fonts[@hero.getPlayer==$map.getPlayer]
		if @font!=@oldfont
			puts "#{@hero.getPlayer} #{$map.getPlayer} #{@oldPlayer}"
			puts "#{@font} #{@oldfont}"
			@oldPlayer=@hero.getPlayer
			puts "font changed"
			$fontChangeCount||=0
			$fontChangeCount+=1
			if $fontChangeCount>20
				raise 1
			end
			@oldfont=@font
			@textWidget.setFont(@font)
			queryRedraw
		end
		super
	end

	private
end

class AntInfoBox<AGLayout
	def initialize(p,n)
		super(p)
		loadXML(loadFile("data/gui/layout/infobox.xml"))
		getChild("infotext").setText(n)
	end
end