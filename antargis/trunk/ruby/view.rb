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
		return AGRect.new(sp.x-50,sp.y-30,100,30)
	end
	
	def setupHeroDisplay
		heroes=$map.getOwnHeroes
		if heroes.length>0
			p=heroes[0].getPos2D
			getScene.setCamera(AGVector4.new(p.x,p.y,0))
		end
		setupNames
	end
	
	def setupNames
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

	def playStory(story)
		
	end




	def eventHover(list,button)
		if list.length>0
			n=list[0]
			mesh=n.node
			ok=false
			if mesh.class==Mesh
				ent=getMap.getEntity(mesh)
				if ent
					if ["house","farm","farmstead","workshop","hero"].member?(ent.getType) then
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
		puts "CLICKENTS"
		
		job=$buttonPanel.getJob
		puts "JOB:"+job
		if job=="doPoint" then
			# select
			list.each{|ents|
				mesh=ents.node
				puts mesh
				if mesh.class==Mesh
					e=getMap.getEntity(mesh)
					if not e
						puts mesh.class
						puts toTerrainMesh(mesh)
						if not toTerrainMesh(mesh)
							raise "WHY IS IT NOT IN MAP ????????????"
						end
					else
						if e.getType=="hero" then
							inspectEntity(e)
							if e.getPlayer==getMap.getPlayer
								@hero=e
								playSound("mylord")
							else
								@hero=nil
							end
							break
						elsif ["house","farm","farmstead","workshop"].member?(e.getType) then
							inspectEntity(e)
							break
						end
					end
				end
			}
			return
		elsif job=="doMove" then
			# FIXME: exchange with good position estimation
			doMove(list[0].node.getPos2D)
		elsif job=="doRecruit" then
			displayError("not allowed")
			puts "RECRUITING"
			if @hero then
				# get house
				house=nil
				list.each{|ents|
					if ents.node.class==Mesh
						e=getMap.getEntity(ents.node)
					#e=getMap.getRuby(ents.get)
					if e.getType=="house" then
						house=e
					end
					end
				}
				if house then
					puts "DOING RECRUIT"
					puts house
					@hero.newHLRecruitJob(house)
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
							if e.getType=="hero" or e.getType=="house" then
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
class AntInventory<AGButton
	def initialize(p,rect)
		super(p,rect,"")
		setTheme("antButton")
		$inventory=self
		@resTypes=["wood","stone","men","food","tool"]
		setEnabled(false)
	end
	def setValue(name,value)
		ok=@resTypes+["name","boss"]
		if ok.member?(name) then
			w=toAGEdit(getChild(name))
			if value.class==String
				w.setText value
			else
				w.setText value.to_i.to_s
			end
		end
	end
	def setTitle(t)
		toAGEdit(getChild("invTitle")).setText(t)
	end
	def inspect(e)
		setTitle(e.getType)
		@inspect=e
	end
	def draw(p)
		updateInspection
		super(p)
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
		@jobButtons=["doPoint","doMove","doFight","doRecruit","doDismiss","doInvent"]
		@aggButtons={"doAgg0"=>1,"doAgg1"=>2,"doAgg2"=>3}
		@inited=false
		@agg=1
	end
	def init
		toAGButton(getChild("doAgg0")).setChecked(true)
		toAGButton(getChild("doPoint")).setChecked(true)
		@job="doPoint"
		@jobButtons.each {|b|
			c=getChild(b)
			puts c
			addHandler(c,:sigClick,:sigJobSelected)
		}
		@aggButtons.each {|b,a|
			puts b
			c=getChild(b)
			puts c
			addHandler(c,:sigClick,:sigAggSelected)
		}
	end
	def draw(p)
		super(p)
		if not @inited then
			@inited=true
			init
		end
	end
	
	def sigJobSelected(e)
		
		@job=e.getCaller.getName
		
		if @job=="doDismiss" then
			$antView.doDismiss
		end
		return true
	end
	def sigAggSelected(e)
		@agg=@aggButtons[e.getCaller.getName]
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
		toAGButton(getChild("doPoint")).setChecked(true)
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
		@cr.setH(@cr.height/2)
		@cr.setY(@cr.height)
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
		@cr1=AGRect.new(@cr.getX,@cr.getY,(@cr.width*@energy).to_i,@cr.height/2)
		@cr2=AGRect.new(@cr.getX,@cr.getY+@cr.height/2,(@cr.width*@energy).to_i,@cr.height/2)
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