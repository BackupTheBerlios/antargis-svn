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
	end
	def clickMap(pos)
		if @hero then
			@hero.newHLMoveJob(0,pos,0)
		end
	end
	def clickEntities(list)
		puts "CLICKENTS"
		
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
		
		# first take only first
		if list.length>0 then
			e=list[0].get
			inspectEntity(e)
			if e.getType=="hero" then
				@hero=$map.getById(e)
				puts "HERO"
				puts @hero
			end
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
		@resTypes=["wood","stone"]
	end
	def setValue(name,value)
		if name=="wood" or name=="stone" then
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
