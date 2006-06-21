#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ents.rb
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

require 'ant_local.rb'

# this variables are used for playing sounds, which should be done with AntRubyEntity::playSound
INNER_VOL_SIZE=9 # size of circle around camera-middle with max volume
OUTER_VOL_SIZE=25 # size circle around camera describing volume descend

# AntRubyEntity slighty enhances the functionality already provided by AntEntity.
# This should be used for all the entity-types.
class AntRubyEntity<AntEntity
	attr_accessor :birthday
	def initialize(p)
		super(p)
		@xmlProps={}
		@birthday=getMap.getTime
		@mode=""
		@handlers={}

		setHunger(0) # general entities have no hunger
	end
	def setXMLProp(n,v)
		@xmlProps[n]=v
	end
	def getXMLProp(n)
		if @xmlProps[n]==nil
			return ""
		else
			return @xmlProps[n]
		end
	end
	def loadXML(node)
		super
		if node.get("birthday")!=""
			@birthday=node.get("birthday").to_f
		end
		@mode=node.get("mode")
	end
	def saveXML(node)
		super(node)
		@xmlProps.each{|n,v|
			node.set(n,v)
		}
		if @birthday
			node.set("birthday",@birthday.to_s)
		end
		node.set("mode",@mode)
	end
	def getDescription
		"This is an entity - no more info here."
	end

	# simple comparison operator, so that ents can be distinguished
	def <=>(e)
		to_s<=>e.to_s
	end
	def menCount
		0
	end
	def getPlayer
		nil
	end
	def fightTarget
		@fightTarget
	end
	def getRand
		# FIXME - implement me (network code)
		rand
	end

	def setStrength(v)
		super
		setMoraleStrength(v*2)
	end
	def age
		((getMap.getTime-@birthday).to_f/$YEAR).to_i
	end
	def age=(years)
		@birthday=getMap.getTime-years*$YEAR
	end
	def getMen
		[]
	end
	def setMode(mode)
		@mode=mode
	end
	def getMode
		@mode
	end

	def playSound(name,minDiff=0.5)
		d=(($app.getScene.getCamera.dim2-getPos2D).length-INNER_VOL_SIZE)
		vol=1
		if d>0
			vol=[(OUTER_VOL_SIZE-d)/OUTER_VOL_SIZE,0].max
		end
		playSoundGlobal(name,vol,minDiff)
	end

# 	def playLoopSound(name)
# 		s={"fire"=>"data/sound/fire.wav"}[name]
# 		return nil if s.nil?
# 		d=(($app.getScene.getCamera.dim2-getPos2D).length-INNER_VOL_SIZE)
# 		vol=1
# 		if d>0
# 			vol=[(OUTER_VOL_SIZE-d)/OUTER_VOL_SIZE,0].max
# 		end
# 		getSoundManager.loopPlay(s,0.4*vol)
# 	end

	def isOnOpenWater(p=nil)
		if (not p)
			p=getPos2D
		end
		getMap.getPos(p).z<-0.2
	end
	def isOnWater
		getMap.getPos(getPos2D).z<0
	end
	def xmlName
		xml=self.class.to_s
		xml=xml[0..0].downcase+xml[1..1000]
		return xml
	end

	def addHandler(eventName,&block)
		@handlers[eventName]||=[]
		@handlers[eventName].push(block)
	end

	def doEvent(name)
		if @handlers[name]
			@handlers[name].each{|b|
				b.call
			}
		end
	end

	def newFightJob(p,target,distance)
		@fightTarget=target
		super
		doEvent(:eventNewFightJob)
	end
	def newRestJob(t)
		super
		doEvent(:eventNewRestJob)
	end
	def eventNoJob
		super
		doEvent(:eventNoJob)
	end
	def eventJobFinished
		super
		doEvent(:eventJobFinished)
	end
end

# here comes a list of all the different entity-types BoA currently uses:
require 'ant_hero.rb'
require 'ant_sheep.rb'
require 'ant_man.rb'
require 'ant_house.rb'
require 'ant_stone.rb'
require 'ant_tree.rb'
require 'ant_deco.rb'
require 'ant_tower.rb'
require 'ant_dwelling.rb'
require 'ant_farm.rb'
require 'ant_workshop.rb'
require 'ant_fir.rb'
require 'ant_grass.rb'
require 'ant_druid.rb'
require 'ant_decal.rb'
require 'ant_sack.rb'
require 'ant_mine.rb'
require 'ant_boat.rb'
require 'ant_fish.rb'
require 'ant_arrow.rb'
