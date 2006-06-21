#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# dialogs.rb
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
class AntEditPropDialog<AntDialog
	def initialize(parent,ent)
		super(parent,"data/gui/layout/editpropdialog.xml")
		@ent=ent
		menCountW=toAGEdit(getChild("MenCount"))
		nameW=toAGEdit(getChild("AntName"))
		menCountW.setText(@ent.getXMLProp("men"))
		nameW.setText(@ent.getName)
		
		getChild("NpcTypeView").hide if @ent.class!=AntNPC
		if @ent.class==AntNPC
			@npcTypeW=toAGEdit(getChild("NpcType"))
			@npcTypeW.setText(@ent.npcType)
		end
	end
	def eventOk(e)
		menCountW=toAGEdit(getChild("MenCount"))
		nameW=toAGEdit(getChild("AntName"))
		@ent.setName(nameW.getText)
		@ent.setXMLProp("men",menCountW.getText)
		close
		@ent.npcType=@npcTypeW.getText if @ent.class==AntNPC
		@ent.setupMesh
		return true
	end
end


class AntEditGeneratorDialog<AntDialog
	attr_reader :edits
	def initialize(parent,params)
		super(parent,"data/gui/layout/editor_generator.xml")
		addSignal("sigGenerate")

		@params=params
		@edits={}

		frame=getChild("frame")

		table=AGTable.new(frame,frame.getRect.origin)

		table.addColumn(1)
		table.addColumn(1)
		params.params.each{|p|
			table.addRow(1)
		}
		y=0
		#table.arrange
		params.params.each{|p|
			n=AGText.new(table,AGRect2.new(0,0,80,20),p.to_s+":",AGFont.new("Arial.ttf",20))
			#n.setAlign(AGText::RIGHT)
			#n=AGButton.new(table,AGRect2.new(0,0,80,20),"huhuhu")
			table.addChild(0,y,n)
			e=AGEdit.new(table,AGRect2.new(0,0,80,20))
			edits[p]=e
			e.setText(eval("params.#{p}").to_s)
			table.addChild(1,y,e)

			y+=1
		}
		table.arrange

		frame.addChild(table)
	end
	def eventOk(e)
		close
		e.setName("sigGenerate")
		sigGenerate(e)
		return true
	end
end

