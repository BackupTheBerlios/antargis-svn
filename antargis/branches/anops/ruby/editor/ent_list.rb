#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ent_list.rb
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

class AntEntListWidget<AGWidget
	attr_reader :entType
	def initialize(p,r)
		super(p,r)
		@entType=nil
		@classes=getDescendantsOfClass(AntRubyEntity)
		@wptrs={}
		if false		
			@classes=@classes.select{|n|
				fn="data/gui/editor/entities/#{n}.png"
				File.exists?(fn)
			}
		end

		group=AGRadioGroup.new(self,getRect.origin)
		group.setName("EntListGroup")
		addChild(group)
		cells=(Math::sqrt(@classes.length)+1).to_i
		table=AGTable.new(group,getRect.origin)
		@table=table
		(1..cells).each{|n|
			table.addColumn(1)
			table.addRow(1)
		}

		i=0
		@classes.each{|c|
			x=i%cells
			y=i/cells
			n=c.to_s
			puts "#{i} #{x} #{y} #{n}"
			w=AGRadio.new(table,AGRect2.new(0,0,40,40))
			@wptrs[c]=w
			w.setName(n)
			w.setTooltip(n)
			addHandler(w,:sigClick,:eventSelected)
		
			fn="data/gui/editor/entities/#{n}.png"
			if File.exists?(fn)
				w.setSurface(AGSurface.load(fn))
			else
				w.setEnabled(false)
			end

			table.addChild(x,y,w)
			i+=1
		}
		table.arrange
		group.addChild(table)

		setCaching(true)
		addSignal("sigSelected")
	end
	def eventSelected(e)
		c=eval(e.getCaller.getName)
		puts c
		c
		@entType=c
		@selWidget=e.getCaller
		sigSelected(e)
		return true
	end
	def entType=(c)
		@wptrs[c].setChecked(true)
		@entType=c
	end
	def deselect
		if @selWidget
			@selWidget.deselect
		end
	end
end

class AntEntListCreator<AGLayoutCreator
	def initialize()
		super("antEntList")
	end
	def create(parent,rect,node)
		e=AntEntListWidget.new(parent,rect)
		e.setName(node.get("name"))
		puts node.get("name")
		#raise 1
		return e
	end
end


$antEntListCreator=AntEntListCreator.new
