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
		return AGRect.new(sp.x-50,sp.y-45,100,40)
	end

	def setupHeroDisplay(first=false)
		# init
		heroes=$map.getOwnHeroes
		if heroes.length>0
			h=heroes[0]
			selectHero(h)
			if first
				focusHero(h)
			end
		end
		if first
			setupNames
		end
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
	end
	
	def eventClick(list,button)
		list.each{|l|
			puts l.node.to_s+"\t"+l.node.class.to_s+"  "+l.camDist.to_s
			if l.node.class==Mesh
				puts getMap.getEntity(l.node)
			end
		}
		puts "---"
		
		if list.length>0
			first=list[0]
			if [TerrainPieceVA,WaterPiece].member?(first.node.class) # == TerrainPieceVA or first.ntoTerrainMesh(first.node)==getMap.getTerrainMesh
				clickMap(first.pos,button)
			else
				clickEntities(list,button)
			end
		end
	end
	def clickMap(pos,button)
		puts "CLICKMAP"
		if @hero then
			@hero.newHLMoveJob(0,pos,0)
		end
	end

	def clickEntities(list,button)
		puts "clickEntities"
		puts button

		# find first entity
		ent=nil
		list.each{|node|
			mesh=node.node
			if [Mesh,AnimMesh].member?(mesh.class)
				ent=getMap.getEntity(mesh)
				break if ent
			end
		}
		if ent
			if ent.class==AntHero and ent.getPlayer==getMap.getPlayer
				@hero=ent
			else
				@target=ent
				inspectEntity(ent)
			end
		end

		if button==1
			# left button == select
		elsif button==3
			# right button == fight
			if ent==@target
				@hero.newHLFightJob(target)
			end
		end
	end

	def clickEntitiesOld(list,button)
		puts "CLICKENTS"
		puts $buttonPanel
		job=$buttonPanel.getJob
		if job==nil
			return
		end
		puts "JOB:"+job
		if job=="doPoint" then
			# select
			list.each{|ents|
				mesh=ents.node
				puts mesh
				if mesh.class==Mesh or mesh.class==AnimMesh
					e=getMap.getEntity(mesh)
					if not e
						puts mesh.class
						puts toTerrainMesh(mesh)
						if not toTerrainMesh(mesh)
							raise "WHY IS IT NOT IN MAP ????????????"
						end
					else
						if e.class==AntHero then
							inspectEntity(e)
							if e.getPlayer==getMap.getPlayer
								@hero=e
								playSound("mylord")
							else
								@hero=nil
							end
							break
						elsif e.is_a?(AntEntity) then  # was AntBoss
							inspectEntity(e)
							break
						end
					end
				end
			}
			return
		elsif job=="doMove" then
			# FIXME: exchange with good position estimation
			mesh=list[0].node
			e=getMap.getEntity(mesh)
			if e
				doMove(e.getPos2D)
			end
		elsif job=="doRecruit" then
			#displayError("not allowed")
			puts "RECRUITING"
			if @hero then
				# get house
				house=nil
				list.each{|ents|
					if ents.node.class==Mesh
						e=getMap.getEntity(ents.node)
						if e.is_a?(AntHouse) then
							house=e
						end
					end
				}
				if house then
					puts "DOING RECRUIT"
					puts house
					@hero.newHLRecruitJob(house)
					$buttonPanel.setPointing
				end
			end
		elsif job=="doFight" then
			if @hero then
				target=nil
				puts "LISTLEN:",list.length
				list.each{|ents|
					puts "ITERATION"
					if ents.node.class==Mesh
						e=getMap.getEntity(ents.node)
						if not e
							if not toTerrainMesh(ents.node)
								raise "Why is this not in the map????"
							end
						else
							if e.is_a?(AntBoss) then
								target=e
							end
						end
					end
				}
				if target then
					@hero.newHLFightJob(target)
					# go back to "normal" pointing
					$buttonPanel.setPointing
				end
			end
		end
	end
	
	def doMove(pos)
		if @hero then
			#pos=list[0].get.getPos2D
			@hero.newHLMoveJob(0,pos,0)
		end
	end
	def doDismiss()
		if @hero
			@hero.newHLDismissJob
		end
		$buttonPanel.setPointing
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
	
	def inspectEntity(e)
		if @inspect
			if @inspect.is_a?(AntBoss)
				@inspect.selected=false
			end
		end
		@inspect=e
		if @inspect
			if @inspect.is_a?(AntBoss)
				@inspect.selected=true
			end
		end
		$inventory.inspect(e)
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


# Inventory view
class AntInventory<AGWidget
	def initialize(p,rect)
		super(p,rect)
		$inventory=self
		@resTypes=["wood","stone","men","food","tool"]
		setCaching(true)
		@invinited=false
	end

	def eventInspect
		$app.viewInformation(@inspect)
	end

	def setValue(name,value)
		ok=@resTypes+["boss"]
		if ok.member?(name) then
			w=getChild(name)
			if value.class==String
				w.setText value
			else
				w.setText value.to_i.to_s
			end
		end
	end
	def setTitle(t)
		c=getChild("invTitle")
		if c.class==AGButton
			c.setCaption(t)
		else
			c.setText(t)
		end
	end
	def inspect(e)
		setTitle(e.class.to_s) #getType)
		@inspect=e
	end
	def draw(p)
		updateInspection
		super(p)
		if not @invinited
			addHandler(getChild("doInspect"),:sigClick,:eventInspect)
			@invinited=true
		end

	end
	def updateInspection
		if @inspect then
			res=@inspect.resource.getAll
			res["men"]=@inspect.menCount.to_f
			reset
			res.each{|a,b|
				setValue(a,b)
			}
			setValue("name",@inspect.getName)
			bossname=""
			if @inspect.methods.member?("getPlayer")
				if @inspect.getPlayer
					bossname=@inspect.getPlayer.getName
				end
			end
			setValue("boss",bossname)
		end
	end
	def reset
		@resTypes.each{|x|setValue(x,0)}
	end
end

class AntInventoryCreator<AGLayoutCreator
	def initialize()
		super("antInventory")
	end
	def create(parent,rect,node)
		return AntInventory.new(parent,rect)
	end
end
$antInventoryCreator=AntInventoryCreator.new


class AntButtonPanel<AGWidget
	include AGHandler
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
	def init
		toAGButton(getChild("doAgg0")).setChecked(true)
		toAGButton(getChild("doFight")).setChecked(true)
		@job="doPoint"
		@jobButtons.each {|b|
			c=getChild(b)
			puts b
			puts c
			addHandler(c,:sigClick,:eventJobSelected)
		}
		@aggButtons.each {|b,a|
			puts b
			c=getChild(b)
			puts c
			addHandler(c,:sigClick,:eventAggSelected)
		}
	end
	def draw(p)
		super(p)
		if not @inited then
			@inited=true
			init
		end
	end
	
	def eventJobSelected(e)
		
		@job=e.getCaller.getName
		sigJobChanged(e)
		#if @job=="doDismiss" then
		#	$antView.doDismiss
		#end
		return true
	end
	def eventAggSelected(e)
		@agg=@aggButtons[e.getCaller.getName]
		sigAggressionChanged(e)
		return true
	end
	
	def getJob()
		return @job
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
		@name=@hero.getName
		addChild(@textWidget=AGText.new(self,AGRect.new(0,0,width,height/2),@hero.getName,@font))
		addChild(b=AGButton.new(self,AGRect.new(0,height/2,width,height/2-1),""))
		b.setTheme("antButton")
		@energy=0.7
		updateRects
	end
	
	def setEnergy(e)
		@energy=e
		updateRects
	end

	def setRect(r)
		super(AGRect.new(r.x.to_i,r.y.to_i,r.w.to_i,r.h.to_i))
	end
	
	def drawAfter(p)
		setEnergy(@hero.getEnergy)
		updateColor
		a0=AGColor.new(0,0x7f,0)
		a1=AGColor.new(0,0x7f,0)
		a2=AGColor.new(0,0xff,0)
		a3=AGColor.new(0,0xAf,0)
		p.drawGradient(@cr1,a0,a1,a2,a3)
		p.drawGradient(@cr2,a2,a3,a0,a1)
	end
	def getText
		@name
	end
	private
	def updateRects
		@cr1=AGRect.new(@cr.x.to_i,@cr.y.to_i,(@cr.width*@energy).to_i,@cr.height/2)
		@cr2=AGRect.new(@cr.x.to_i,(@cr.y+@cr.height/2).to_i,(@cr.width*@energy).to_i,@cr.height/2)
	end
	def updateColor
		if @hero.getPlayer==getMap.getPlayer
			f=getTheme.getFont("heroName.font")
		else
			f=getTheme.getFont("enemyHero.font")
		end
		@textWidget.setFont(f)
	end
end