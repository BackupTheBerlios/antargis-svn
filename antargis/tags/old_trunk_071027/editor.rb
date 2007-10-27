#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# game.rb
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

$programDir=Dir.pwd+"/ruby"
# add programdir to path
$:.push($programDir)

require 'antargislib.rb'

require 'dialogs.rb'
require 'ents.rb'
require 'map.rb'
require 'view.rb'
require 'editview.rb'
require 'map_generator.rb'
require 'editor/dialogs.rb'


def getSavePath
	"data/levels"
end

class AntGameApp < AntRubyEditView
	def initialize(sw,sh)
		super(sw,sh,nil) #AntRubyMap.new(64,64))
		$app=self	
		$map=@map=AntRubyMap.new(getScene,128,128)
		$map.setHeight(-0.5)

		addHandler(@layout.getChild("quit"),:sigClick,:eventQuit)
		
		addHandler(@layout.getChild("doGen"),:sigClick,:eventGenerateDialog)

		addHandler(@layout.getChild("loadButton"),:sigClick,:load)
		addHandler(@layout.getChild("saveButton"),:sigClick,:save)

		addHandler(@layout.getChild("playerButton"),:sigClick,:editPlayers)
		
		@miniMap=toMiniMap(@layout.getChild("miniMap"))
		@miniMap.setMap(getMap)
		@miniMap.setScene(getScene)
		
		$screen=@layout
		
		#eventGenerate
	end

	def eventGenerateDialog
		@layout.addChild(dialog=AntEditGeneratorDialog.new(@layout,GeneratorParameters.new))
		addHandler(dialog,:sigGenerate,:eventGenerate)
		@generatorDialog=dialog
	end
	def eventGenerate
		map=getMap
		getMap.setHeight(-0.5)
		ps=GeneratorParameters.new
		ps.readFromDialog(@generatorDialog)

		if true
			generateMap(getMap,ps)
		end
		return true
	end
	
	def eventFrame(time)
		super(time)
		getScene.advance(time)
		return true
	end
	
	def eventIdle
		delay(20)
		return true
	end
	
	# signals	
	def eventQuit(e)
		@layout.addChild(AntQuitDialog.new(@layout))
		return true
	end
	def eventPause(e)
		if not getMap().paused then
			@layout.addChild(AntPauseDialog.new(@layout))
		else
			getMap().unpause
		end
		return true
	end
	def eventOptions(e)
		@layout.addChild(AntOptionsDialog.new(@layout))
		return true
	end

	def editPlayers
		@layout.addChild(AntPlayerEditDialog.new(@layout))
		return true
	end
	
	def enableEdit
		toEditIsoView(@layout.getChild("mainView")).toggleEdit
	end
	def save
		@layout.addChild(AntSaveDialog.new(@layout))
	end
	def load
		@layout.addChild(AntLoadDialog.new(@layout))
	end
	
end

app=AntGameApp.new(1024,768)
if ARGV[0]
	getMap.loadMap(ARGV[0]+".antlvl")
end

app.run

