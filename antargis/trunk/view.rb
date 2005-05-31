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


class AntRubyView <CompleteIsoView
	def initialize(p,rect,pos,map)
		super(p,rect,pos,map)
		$antView=self
	end
	def clickMap(pos)
		if @hero then
			@hero.newHLMoveJob(0,pos,0)
		end
	end
	def clickEntities(list)
		puts "CLICKENTS"
		
		job=$buttonPanel.getJob
		puts "JOB:"+job
		if job=="doPoint" then
			# select
			list.each{|ents|
				e=getMap.getRuby(ents.get)
				if e.getType=="hero" then
					@hero=e
					inspectEntity(e)
				elsif e.getType=="house" then
					inspectEntity(e)
				end
			}
			return
		elsif job=="doMove" then
			doMove(list[0].get.getPos2D)
		elsif job=="doRecruit" then
			puts "RECRUITING"
			if @hero then
				# get house
				house=nil
				list.each{|ents|
					e=getMap.getRuby(ents.get)
					if e.getType=="house" then
						house=e
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
				list.each{|ents|
					e=getMap.getRuby(ents.get)
					if e.getType=="hero" or e.getType=="house" then
						target=e
					end
				}
				if target then
					@hero.newHLFightJob(target)
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

class AntRubyEditView<EditIsoView
	def editMarkClicked(p,e)
		
	end
end


class AntRubyViewCreator<AGLayoutCreator
	def initialize()
		super("antRubyView")
	end
	def create(parent,rect,node)
		w=AntRubyView.new(parent,rect,Pos3D.new(0,0,0),$map)
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
		@resTypes=["wood","stone","men"]
		setEnabled(false)
	end
	def setValue(name,value)
		if name=="wood" or name=="stone" or name=="men" then
			w=toAGEdit(getChild(name))
			w.setText value.to_s
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
			res["men"]=@inspect.menCount
			#$inventory.setTitle(e.getType)
			reset
			res.each{|a,b|
				$inventory.setValue(a,b)
			}
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
		puts "JOB:"+@job
		
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