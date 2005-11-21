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
	def eventMouseButtonUpTest(m)
		puts "MUH"
		e=toAGSDLEvent(m)
		p=e.getMousePosition
		#v=AGVector2.new(p.x/getScene.width,p.y/getScene.height)
		l=getScene.pick(p.x,p.y,1,1)
		l.each{|o|
			puts o.node.to_s+"  "+o.camDist.to_s+"   "+o.pos.to_s
		}
		#getScene.pick(m
		
		
		exit
		super(m)
	end
	
	def eventHover(list,button)
		if list.length>0
			n=list[0]
			mesh=n.node
			if mesh.class==Mesh
				ent=getMap.getEntity(mesh)
				if ent
					if ["house","farm","farmstead","workshop","hero"].member?(ent.getType) then
						#puts ent.class
						#exit
					#if ent.getType=="hero"
						inspectEntity(ent)
					end
				end
			end
		end
	end
	
	def eventClick(list,button)
		puts "EVENTCLICK"
		list.each{|l|
			puts l.node.to_s+"\t"+l.node.class.to_s+"  "+l.camDist.to_s
			if l.node.class==Mesh
				puts getMap.getEntity(l.node)
			end
		}
		puts "---"
		
		if list.length>0
			first=list[0]
			#puts first.node
			#puts first.node.class
			#puts "FIRST:",toTerrainMesh(first.node), getMap.getTerrainMesh
			if toTerrainMesh(first.node)==getMap.getTerrainMesh
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
							@hero=e
							inspectEntity(e)
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
	
	def inspectEntity(e)
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
			w.setText value.to_i.to_s
		end
	end
	def setTitle(t)
		toAGButton(getChild("invTitle")).setCaption(t)
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
	
	def sigJobSelected(ename,cname,event,caller)
		@job=cname
		
		if @job=="doDismiss" then
			$antView.doDismiss
		end
		return true
	end
	def sigAggSelected(ename,cname,event,caller)
		@agg=@aggButtons[cname]
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
