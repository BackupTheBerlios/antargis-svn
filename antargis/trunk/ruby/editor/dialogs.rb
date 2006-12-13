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
		menCountW.setText(AGStringUtf8.new(@ent.getXMLProp("men")))
		nameW.setText(AGStringUtf8.new(@ent.getName))
		
		getChild("NpcTypeView").hide if @ent.class!=AntNPC
		if @ent.class==AntNPC
			@npcTypeW=toAGEdit(getChild("NpcType"))
			@npcTypeW.setText(AGStringUtf8.new(@ent.npcType))
		end

		# init player-list
		plist=getChild("Player")
		players=getMap.players
		playernames=["NONE"]
		playernames+=players.collect{|p|p.getName} if ent.is_a?(AntBoss)
		playernames.each{|n|
			plist.insertItem(n,AGStringUtf8.new(n))
		}
		if ent.is_a?(AntBoss)
			if ent.getPlayer
				plist.setSelected(ent.getPlayer.getName)
			end
		end


	end
	def eventOk(e)
		menCountW=toAGEdit(getChild("MenCount"))
		nameW=toAGEdit(getChild("AntName"))
		@ent.setName(nameW.getText.to_s)
		@ent.setXMLProp("men",menCountW.getText.to_s)
		close
		@ent.npcType=@npcTypeW.getText.to_s if @ent.class==AntNPC
		@ent.setupMesh
		if @ent.is_a?(AntBoss)
			name=getChild("Player").getSelected
			@ent.setPlayer(getMap.players.find{|n|n.getName==name})
		end
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
			n=AGText.new(table,AGRect2.new(0,0,80,20),AGStringUtf8.new(p.to_s+":"),AGFont.new("FreeSans.ttf",20))
			#n.setAlign(AGText::RIGHT)
			#n=AGButton.new(table,AGRect2.new(0,0,80,20),"huhuhu")
			table.addChild(0,y,n)
			e=AGEdit.new(table,AGRect2.new(0,0,80,20))
			edits[p]=e
			e.setText(AGStringUtf8.new(eval("params.#{p}").to_s))
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



class AntPlayerEditDialog<AGLayout
	include AGHandler
	def initialize(parent)
		super
		loadXML(loadFile("data/gui/layout/editor_players_dialog.xml"))
		@players={}
		@list=getChild("playerList")
		initPlayerNames
		initPlayerTypes

		if @players.length>0
			eventPlayerSelected(nil)
		end
			
		addHandler(@list,:sigSelect,:eventPlayerSelected)
		addHandler(getChild("addPlayer"),:sigClick,:eventAddPlayer)
		addHandler(getChild("changePlayer"),:sigClick,:eventChangePlayer)
		addHandler(getChild("deletePlayer"),:sigClick,:eventDelPlayer)
	end

	def eventPlayerSelected(e)
		name=@list.getSelectedID
		getChild("playerName").setText(name)
		getChild("playerType").selectItem(@players[name].class.to_s)
		return true
	end

	def eventAddPlayer(e)
		# check if already a player of this name exists
		name=getChild("playerName").getText
		if @players.keys.member?(name)
			messageBox("Error","Please enter a new player-name. This already exists!",MB_OK)
		else
			type=eval(getChild("playerType").getSelectedID)
			player=type.new(name)
			getMap.players.push(player)
			initPlayerNames
		end
		return true
	end

	def eventChangePlayer(e)
		name=getChild("playerName").getText
		sel=@list.getSelectedID
		if @players.keys.member?(name) and sel!=name
			messageBox("Error","Please enter a new player-name. This already exists!",MB_OK)
		else
			@players[sel].name=name
			type=eval(getChild("playerType").getSelectedID)
			if type!=@players[sel].class
				# argh, assigned new player type - delete old one and generate new
				getMap.players.delete(@players[sel])
				player=type.new(name)
				getMap.players.push(player)
			end
			initPlayerNames
		end
		return true
	end
	
	def eventDelPlayer(e)
		sel=@list.getSelectedID
		if @players[sel]
			getMap.players.delete(@players[sel])
			initPlayerNames
		end
		return true
	end

private
	def initPlayerNames
		@list.clearList
		@players={}
		getMap.players.each{|p|
			@list.insertItem(p.name,p.name)
			@players[p.name]=p
		}
	end

	def initPlayerTypes
		@types=getDescendantsOfClass(AntPlayer)
		l=getChild("playerType")
		l.clearList
		@types.each{|t|
			l.insertItem(t.to_s,AGStringUtf8.new(t.to_s))
		}
	end
end

