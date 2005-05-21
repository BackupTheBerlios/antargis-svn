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

# try to implement alle entites in ruby
# WARNING: DON'T MEMBER_VARIABLES AS IT SEEMS TO CRASH RUBY SOMEHOW
# could be that it has something to do with Init_Stack ???


class AntMyEntity<AntEntity
	def initialize(p)
		super(p)
		mRUBY=self
	end
end

module FightHLEntity
	def gotHLFight(target)
		@fightStarted=false
		@fightTarget=target
		@fighting=true
		checkFight
	end
	def doHLFight(target)
		@fightTarget=target
		@fighting=true
	end
	def endFight
		@fightTarget=nil
		@fighting=false
		@fightStarted=false
	end
	def wonFight
		puts "WON FIGHT!!!!!!!!!!!!"
		endFight
	end
	def setOwner(owner)
		@owner=owner
	end
	def lostFight
		puts "LOST FIGHT!!!!!!!!!!!!"
		setOwner(@fightTarget)
		endFight
	end
	def checkFight()
		if @fighting then
			tmen=@fightTarget.undefeatedMen
			if tmen.length>0 then
				umen=undefeatedMen
				if umen.length>0 then
					umen.each{|m|
						if @fightStarted==false or m.hasJob==false then
							if @defeated.include?(m) then
								puts "ERROR!"
								exit
							end
							# search nearest enemy
							dist=10000.0
							mtarget=nil
							tmen.each{|t|
								d=(t.getPos2D-m.getPos2D).norm
								if d<dist
									dist=d
									mtarget=t
								end
							}
							if mtarget then
								m.newFightJob(0,mtarget)
							end
						end
						#if not @defeated.include?(m) then
						#	tindex=(rand*tmen.length).to_i
						#	if @fightStarted==false or tmen[tindex].hasJob==false then
						#		m.newFightJob(0,tmen[tindex])
						#	end
						#end
					}
					@fightStarted=true
				else
					lostFight
				end
			else
				wonFight
			end
		end
	end
	def undefeatedMen
		un=@men-@defeated
		#puts "undefeated:"+un.to_s
		return un
	end
	def sigDefeated(man)
		@defeated.push(man)
	end
end


require 'ant_hero.rb'
require 'ant_sheep.rb'
# MAN

class AntNewAngel<AntMyEntity
	def initialize
		super(Pos2D.new(0,0))
		setType("angel")
		@age=0
	end
	def move(time)
		p=getPos3D
		p.y+=time*20
		setPos3D(p)
		@age+=time
		if @age>10 then # 10 seconds old
			# discard
			getMap.removeEntity(self)
			getMap.endChange
		end
	end
	def getSurfaceName
		return "angel"
	end
end

class AntNewMan<AntMyEntity
	def initialize()
		super(Pos2D.new(0,0))
		setType("man")
		@signed=false
		@dead=false
		@fighting=false
	end
	def getTexture
		if @dead
			return "grave"
		elsif @fighting
			return "man1_sword1"
		else
			return "man"+mDirNum.to_s
		end
	end
	
	def noJob
		jobFinished
	end
	
	def setNoBoss()
		setVar("bossName","")
	end
	
	def setBoss(hero)
		setVar("bossName",hero.getName)
		hero.signUp(self)
	end
	
	#def gotNewJob
	#	puts "GOTNEWJOB:"+self.to_s
	#end
	
	def sendAngel
		e=AntNewAngel.new
		e.setPos2D(getPos2D)
		getMap.insertEntity(e)
		getMap.endChange
	end
	
	#def hit(sub)
	#	
	#end
	def simDie
		puts "SIMDIE"
		puts self
		if not @dead
			@dead=true
			updateSurface
		end
		sendAngel
	end
	def simDie2
		puts "SIMDIE2"
		puts self
		if @dead
			puts "SIMDIE"
			puts self
			puts hasJob
			if @boss
				@boss.removeMan(self)
			end
			getMap.removeEntity(self)
			getMap.endChange
			return 
		else
			@dead=true
			updateSurface
		end
		sendAngel
	end
	
	def setFighting(v)
		@fighting=v
	end
	
	def defeated
		if @boss then
			@boss.sigDefeated(self)
		end
	end
	
	def die
		simDie
		defeated
		newRestJob(20)
	end
	
	def jobFinished
		super
		if getEnergy==0 or @dead then
			simDie2
			return
			#simDie
			#newRestJob(20)
		end
		
		setFighting(false)
		
		if getVar("bossName")=="" then
		
			house=getMap.getNext(self,"house")
			if house
				houseName=house.getVar("name")
				if houseName=="" then
					puts "ERROR House has no name!"
					exit
				end
				setVar("bossName",houseName)
				newRestJob(rand()*2)
				house=getMap.getRuby(house)
				house.signUp(self)
				@signed=true
			end
		else
			boss=getMap.getByName(getVar("bossName"))
			@boss=boss
			if not @signed then
				puts getVar("bossName")
				boss.signUp(self)
				@signed=true
			end
			boss.assignJob(self)
		end
	end	
	def getSurfaceName
		return "man1"
	end
	def xmlName
		return "antNewMan"
	end
	
	def saveXML(node)
		super(node)
		puts "SAVEXML"
		puts node.methods
		node.set("bossName",getVar("bossName"))#@bossID.to_s)
	end
	def loadXML(node)
		super(node)
		setVar("bossName",node.get("bossName"))
	end
end


class AntNewTree<AntMyEntity
	def initialize()
		super(Pos2D.new(0,0))
		@typeID=0
		setType("tree")
	end
	def setTreeType(t)
		@typeID=t
	end
	def getSurfaceName
		return "tree"+@typeID.to_s
	end
	def getVirtualY
		100
	end
	def xmlName
		return "antNewTree"
	end
	def saveXML(node)
		super(node)
		node.set("typeID",@typeID.to_s)
	end
	def loadXML(node)
		super(node)
		@typeID=node.get("typeID").to_i
	end
end


class AntNewStone<AntMyEntity
	def initialize()
		super(Pos2D.new(0,0))
		@typeID=0
		setType("stone")
	end
	def setTreeType(t)
		@typeID=t
	end
	def getSurfaceName
		return "deco"+@typeID.to_s
	end
	def getVirtualY
		40
	end
	def saveXML(node)
		super(node)
		node.set("typeID",@typeID.to_s)
	end
	def loadXML(node)
		super(node)
		@typeID=node.get("typeID").to_i
	end
	def xmlName
		return "antNewStone"
	end
end


class AntFlag<AntMyEntity
	def initialize
		super(Pos3D.new(0,0,0))
		setType("flag")
		@age=0
	end
	def getSurfaceName
		return "flag"
	end
end


class AntNewHouse<AntEntity
	include FightHLEntity
	def initialize
		super(Pos2D.new(0,0))
		@type=2
		setType("house")
		@men=[]
		@defeated=[]
		@atHome=[]
		@lastBirth=0
	end
	
	def setPos2D(p)
		super(p)
		if @flag then
			p=getPos3D
			p.y+=290
			p.z-=150
			@flag.setPos3D(p)
		end
	end

	def loadXML(node)
		super(node)
		setPos2D(getPos2D)
	end	
	
	def addFlag(owner)
		@flag=AntFlag.new
		setPos2D(getPos2D) # reset flag position
		$map.insertEntity(@flag)
	end
	
	def signUp(man)
		puts "SIGNUP"
		puts man
		@men.push(man)
	end
	
	def removeMan(man)
		@men.delete(man)
		@atHome.delete(man)
	end
	
	def setHouseType(t)
		@type=t
	end
	def getSurfaceName
		return "tower"+@type.to_s
	end
	def getVirtualY
		100
	end
	#def insertMan(m)
	#	@men.push(m)
	#end
	def xmlName
		return "antNewHouse"
	end
	
	def getMen
		@men
	end
	
	def menCount
		@men.length
	end
	
	def takeMan
		m=@men[0]
		removeMan(m)
		return m
	end
	
	def checkBirth
		# only men, which are at home can add to birth rate
		@lastBirth+=@atHome.length
		puts "LASTBIRTH:"+@lastBirth.to_s
		if @lastBirth>50 then
			puts "A MAN IS BORN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
			man=AntNewMan.new
			man.setPos2D(getPos2D)
			getMap.insertEntity(man)
			getMap.endChange
			@lastBirth=-10*rand
		end
	end

	def noJob
		jobFinished
	end
	def jobFinished
		checkBirth
		newRestJob(2)
		if @fighting then
			checkFight
		end
	end
	
	def setOwner(owner)
		super(owner)
		addFlag(owner)
	end
	
	def assignJob(e)
		if @fighting then
			# fight mode
			if @defeated.member?(e)
				# e is defeated - go home and rest there
				if atHome(e)
					e.newRestJob(10)
				else
					e.newMoveJob(0,getPos2D,0)
				end
			else
				checkFight
			end
		elsif atHome(e) then
			@atHome.push(e)
			# is home:
			# 1) take everything from inventory
			resource.takeAll(e.resource)
			# 2) give job
			need=needed()
			# keep at least a third of all men at home
			if need != nil and @atHome.length>@men.length/3 then
				fetch( need[1],need[0],e)
				@atHome.delete(e)
			else
				e.newRestJob(10)
			end
		else
			# is anywhere - come home
			e.newMoveJob(0,getPos2D,0)#,false)
		end
	end
	
	# says if entity is at home
	def atHome(entity)
		pd=entity.getPos2D-getPos2D
		n=pd.norm2
		return n<30
	end
	
	# what's needed most ATM?
	# returns: [good,from] or nil
	def needed()
		goods={"wood"=>"tree","stone"=>"stone"}
		min=10000
		need=nil
		needfrom=nil
		goods.each{|good,from|
			v=resource.get(good)
			if min>v then
				min=v
				need=good
				needfrom=from
			end
		}
		if need==nil then
			return nil
		else
			return [need,needfrom]
		end
	end


	# assigns ent a job for fetching good from a enttype
	def fetch(enttype,good,ent)
		tent=getMap.getNext(self,enttype)
		if tent == nil then
			puts "No '"+good+"' found!"
		else
			ent.newFetchJob(0,tent.getPos2D,good)
		end
	end
end
