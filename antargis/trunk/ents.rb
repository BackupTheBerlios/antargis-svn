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


# MAN
class AntNewMan<AntEntity
	def initialize()
		super(Pos2D.new(0,0))
		setType("man")
		@signed=false
	end
	def getTexture
		return "man"+mDirNum.to_s
	end
	
	def noJob
		jobFinished
	end
	
	#def gotNewJob
	#	puts "GOTNEWJOB:"+self.to_s
	#end
	
	def jobFinished
		super
		
		if getVar("bossID")=="" then
		
			house=getMap.getNext(self,"house")
			setVar("bossID",house.getID.to_s)
			newRestJob(rand()*2)
			house=getMap.getRuby(house)
			house.signUp(self)
			@signed=true
		else
			boss=getMap.getEntity(getVar("bossID").to_i)
			if not @signed then
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
		node.set("bossID",@bossID.to_s)
	end
	def loadXML(node)
		super(node)
		setVar("bossID",node.get("bossID"))
	end
end

class AntHLJob
end

class AntHeroMoveJob<AntHLJob
	def initialize(hero,prio,pos,dist)
		@hero=hero
		@prio=prio
		@pos=Pos2D.new(pos.x,pos.y)#clone
		@dist=dist
		
		format
	end
	def getMen()
		@hero.getMen
	end
	def format
		men=getMen
		@dir=(@pos-@hero.getPos2D).normalized
		men.each{|x|
			fpos=@hero.getWalkFormation(x,@dir)+@hero.getPos2D
			x.newMoveJob(0,fpos,5)
		}
	end
	
	def checkReady
		GC.disable # disable GC, as gets called too often here
		ready=true
		men=getMen
		men.each{|x|
			if x.hasJob then 
				ready=false
			else
				# check if is already there - don't know why this fails ATM
				is=x.getPos2D
				should=@hero.getWalkFormation(x,@dir)+@hero.getPos2D
				if (is-should).norm>10 then
					ready=false
					x.newMoveJob(0,should,0)
				end
			end
		}
		GC.enable
		return ready
	end

	def check
		men=getMen
		ready=checkReady
		if ready then
			#getMap.pause
			# let all go
			@hero.newMoveJob(0,@pos,0)
			men.each{|x|
				fpos=@hero.getWalkFormation(x,@dir)+@pos
				x.newMoveJob(0,fpos,0)
			}
			return true
		end
		return false
	end
end

class AntNewHero<AntEntity
	def initialize
		super(Pos2D.new(0,0))
		@men=[]
		#@resources={}
		setType("hero")
		@job=nil
		
		@createMen=0
	end
	def getSurfaceName
		return "hero1dl"
	end
	def xmlName
		return "antNewHero"
	end
	def saveXML(node)
		super(node)
	end
	def loadXML(node)
		super(node)
		if node.get("men")!="" then
			@createMen=node.get("men").to_i
		end
	end
	
	def noJob
		if @createMen>0
			for i in 0..(@createMen-1) do
				man=AntNewMan.new
				man.setPos2D(getPos2D)
				man.setVar("bossID",getID.to_s)
				getMap.insertEntity(man)
#				puts "LOADXML"
#				puts getID
#				puts getMap.getEntity(getID)
#				puts getMap.getEntity(getID).getID
#				exit
			end
			getMap.endChange
			@createMen=0
		end
	end
	
	def assignJob(e)
		#puts "ASSIGNJOB:"
		#puts e
		#puts self
		if @job == nil then
			e=getMap.getRuby(e)
			formationPos=getSitFormation(e)
			if e.getPos2D==formationPos then
				e.newRestJob(5)
			else
				e.newMoveJob(0,formationPos,0)
			end
		else
#			puts self
			if @job.check then @job=nil end
		end
#		e.newMoveJob(0,getPos2D,0)
	end	
	
	def gotNewJob()
	end
	
	def newHLMoveJob(prio,pos,dist)
		puts "NEWMOVEJOB:"+pos.to_s
		@job=AntHeroMoveJob.new(self,prio,pos,dist)
		
		
	end
	
	def getMen
		@men
	end
	
	def signUp(man)
		puts "HEROSIGNUP"
		puts man
		@men.push(man)
	end
	# formation:
	# 1) wait for 3/4 of people are in formation but max. 5 seconds or so
	# 2) start all at once
	
	def getSitFormation(man)
		id=@men.index(man)
		
		if id then
			angle=id.to_f/@men.length*Math::PI*2
			radius=40
			return Pos2D.new(Math::sin(angle)*radius,Math::cos(angle)*radius)+getPos2D
		else
			puts "ERROR in SitFormation!"
			puts "MEN:"+@men.to_s
			puts "man:"+man.to_s
		end
	end
	
	
	def getWalkFormation(man,dir)
		id=@men.index(man)
		if id
			line=id/5
			col=id%5
			col=col-2
			normal=dir.normal
			l=dir*line*30
			c=normal*col*15
			
#			puts "LINE:"+line.to_s
#			puts "COL:"+col.to_s
#			puts "DIR:"+dir.completeString
#			puts "NORMAL:"+normal.completeString
			
			return l+c
		else
			puts "ERROR in WalkFormation!"
		end
	end
	
end

class AntNewTree<AntEntity
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
	def saveXML(node)
		super(node)
		node.set("typeID",@typeID.to_s)
	end
	def loadXML(node)
		super(node)
		@typeID=node.get("typeID").to_i
	end
end

class AntNewStone<AntEntity
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
end

class AntNewHouse<AntEntity
	def initialize
		super(Pos2D.new(0,0))
		@type=2
		setType("house")
		@men=[]
	end
	
	def signUp(man)
		puts "SIGNUP"
		puts man
		@men.push(man)
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
	def insertMan(m)
		@men.push(m)
	end
	def xmlName
		return "antNewHouse"
	end
	def assignJob(e)
		if atHome(e) then
			# is home:
			# 1) take everything from inventory
			resource.takeAll(e.resource)
			# 2) give job
			need=needed()
			if need != nil then
				fetch( need[1],need[0],e)
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
