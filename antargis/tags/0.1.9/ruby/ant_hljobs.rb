#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hljobs.rb
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

require 'ents.rb'

class UID
	def initialize(id)
		@id=id
	end
	def method_missing(n,*args)
		getMap.getByUID(@id).send(n,*args)
	end
end

class AntHLJob
	def undefeatedMen
		@hero.getMen
	end
	def delete(man)
		if @men
			@men.delete(man)
		end
	end
	def xmlName
		self.class.to_s
	end
	def saveXML(n)
		puts "SAVEXML"
		puts self
		instance_variables.each{|name|
			c=n.addChild("param")
			value=instance_variable_get(name)
			case value
				when AntEntity
					value=value.uid
					type="uid"
					c.set("type","AntEntity")
				when Array
					if value[0].is_a?(AntEntity)
						c.set("type","AntEntities")
						value=value.collect{|v|v.uid.to_s}.join(",")
					end
				when Fixnum,Float,TrueClass,FalseClass
					c.set("type",value.class.to_s)
				when AGVector2
					c.set("type","AGVector2")
				when String
					# do nothing
					c.set("type","String")
				else
					value=AntMarshal.dump(value)
			end
			puts "#{value} #{name}"
			c.set("name",name)
			c.set("value",value.to_s)
		}
	
		#FIXME
	end
	def loadXML(n)
		n.getChildren.each{|c|
			name=c.get("name")
			value=c.get("value")
			type=c.get("type")
			if name and name!=""
				puts "TYPE: #{c.get("type")}"
				case c.get("type")
					when "AntEntity"
						value=UID.new(value.to_i) #getMap.getByUID(value.to_i)
					when "AntEntities"
						value=value.split(",").collect{|v|UID.new(v.to_i)} #getMap.getByUID(v)}
					when "AGVector2"
						x,y=value.gsub("(","").gsub(")","").split(",").collect{|v|v.to_f}
						value=AGVector2.new(x,y)
						puts value
						#raise 1
					when "Fixnum"
						value=value.to_i
					when "Float"
						value=value.to_f
					when "TrueClass"
						value=true
					when "FalseClass"
						value=false
					when "String"
						# do nothing
					else
						puts "unknown type for #{name}"
						value=AntMarshal.load(c.get("value"))
				end
				instance_variable_set(name,value)
			end
		}
		#raise 1
		#FIXME
	end
	def playSound(s)
		@hero.playSound(s)
	end
end

class AntHeroMoveJob<AntHLJob
	def initialize(hero,prio,pos,dist,doFormat=true)
		@hero=hero #getMap.getRuby(hero)
		@hero.delJob
		@prio=prio
		@pos=AGVector2.new(pos.x,pos.y)
		@dist=dist
		@formatDist=0
		if getMen.length>0
			@state="format"
		else
			@state="torest"
			@hero.newMoveJob(0,@pos,@dist)
		end
		@men=getMen
		@men.each{|m|
			m.setMode "format"
		}
		
		@formatDir=(@pos-@hero.getPos2D).normalized
		@moveFinished=false
		@movingMen=0
		@hero.formation=AntFormationBlock.new(@hero,@formatDir)
	end

	def makeMessage(boss)
		MoveMessage.new(boss,@pos,@dist)
	end

	def image
		"data/gui/move.png"
	end
	def getMen()
		@hero.getMen
	end

	def startWalking
		@hero.formation=AntFormationBlock.new(@hero,@formatDir)
		puts "START WALKING"
		@men.each{|m|
# 			f=@pos+@hero.getFormation(m,@formatDir)
			f=@hero.getFormation(m,@pos)
			m.newMoveJob(0,f,@dist)
			m.setMode("moving")
			@movingMen+=1
		}
		@state="move"
	end
	def getMenState(mode)
		@men.select{|m|m.getMode==mode}
	end

	def check(man)
# 		puts "CHECK (move-job): #{man.class.to_s} #{man.getName} #{man.getMode} state:#{@state}"
# 		@men.each{|m|
# 			puts "#{m} #{m.getName} #{m.getMode}"
# 		}
		puts "CHECK #{@state} #{man.getMode}"
		case @state
			when "format"
				if man.getMode=="format"
					f=@hero.getFormation(man,@hero.getPos2D)
					#puts "FORMATTING:#{man} #{f} #{@hero.getPos2D}"
					man.newMoveJob(0,f,@formatDist)
					man.setMode("formating")
				elsif man.getMode=="formatted"
					# some have already waited for 7 seconds
					puts "I'm formatted"
					startWalking
				else
					man.setMode("formatted")
					man.newRestJob(7)
					puts "WAIT"
				end
				if getMenState("formating").length==0
					startWalking
				end
			when "move"
				case man.getMode
					when "moving"
						@movingMen-=1
						man.setMode("torest")
						if @movingMen>0
							man.newRestJob(5)
						end
				end
				puts "movingMen:#{@movingMen}"
				if @movingMen==0
					# FIXME - check if all men are really at destination
					puts "move finished!"
					@moveFinished=true
					#@state="torest"
				end
		end
	end
	
	def moveFinished
		return @moveFinished
	end
	
	def finished
		moveFinished
	end
		
end

class AntHeroFightJob<AntHeroMoveJob
	attr_reader :target

	def initialize(hero,target,defend=false)
		@men=hero.getMen
		@hero=hero
		initSitpositions
		puts "NEW ANTHEROFIGHT JOB #{hero.getName} #{target.getName}"

		@hero=hero
		@target=target
		@killStarted=false
		@killJobsGiven=false
		@defend=defend
		@hero.newRestJob(1)  #FIXME: this is an indirect method of killing actual job
		super(hero,0,target.getPos2D,10,(not defend)) # near til 10

 		if @defend or (not hero.is_a?(AntHero))
			@hero.newRestJob(2) # wait for 3 seconds; then set state to fight - have a look at check() for more
 		end
		@finished=false
	end

	def makeMessage(boss)
		FightMessage.new(@hero.uid,@target.uid,@defend)
	end


	def finished
		@finished
	end
	
	def getEnergy
		raise "deprecated"
		e=0
		@men.each{|m|e+=m.getMorale}
		return e*0.1
	end
	
	def checkFlee
		dist=(@target.getPos2D-@hero.getPos2D).length
		if dist>15 and @state=="fight"
			puts "CHECKSTATE DIST:#{dist}"
			@finished=true
		end
	end
	
	def checkState
		checkFlee
		if @state=="fight" and getMenState("defeated").length==@men.length
			lost
		end
		if moveFinished #or @defend
			
			puts "FightJob::checkState:MOVE FINISHED"
			@state="fight"
			@target.eventAttacked(@hero)
			#startFighting
			#raise "muh" if @hero.class!=AntHero
		end
		if @defend and @target.getJob.class!=self.class
			@finished=true # quick flee
		end
	end
	
	def check(man)
		# if boss is defending and already waited for some seconds, then start fighting!!!
		if man==@hero and @defend
			@state="fight"
			startFighting
		end
		puts "CHECKING #{man} #{man.getName} #{self} #{@hero.getName} #{@state} #{man.getMode}"
		checkState
		return if @finished
		case @state
			when "move","format"
				super(man)
				return false
			when "fight"
				if not @killStarted
					puts "killStarted #{@killStarted} #{self}"
					@target.eventGotHLFight(@hero)
					startFighting
					@killStarted=true
				end
				if man.getMode=="fight"
					return checkFight(man)
				else
					man.newRestJob(50)
				end
		end
	end
	def defeated(man)
		playSound("ugh_end")
		puts "DEFEATED:"+man.to_s
		if man.is_a?(AntBoss)
			lost
			return
		end
		man.setMode("defeated")
		raise "no sit position" if @sitpos[man].nil?
		man.newMoveJob(0,@sitpos[man],0)
	end
	
	def lost
		if not @finished
			dputs "LOST!!!!!!!!!!"
			@finished=true

			if @hero.is_a?(AntHero)
				@hero.eventHLJobFinished(self)
				@hero.newHLMoveJob(0,@sitpos[@hero],0)
			end

			if @target.getJob.is_a?(AntHeroFightJob)
				@target.getJob.won
			end
			@hero.setOwner(@target)
			@hero.killAllJobs
			@target.killAllJobs
		end
	end
	def won
		if not @finished
			dputs "WON!!!!!!!!!!"
			playSound("won")
			@finished=true
			if @hero.is_a?(AntHero)
				@hero.eventHLJobFinished(self)
				@hero.newHLMoveJob(0,@sitpos[@hero],0)
			end

			if @target.getJob.is_a?(AntHeroFightJob)
				@target.getJob.lost
			end
			@hero.killAllJobs
			@target.killAllJobs
		end
	end

	
	def startFighting
		puts "START FIGHTING #{self} #{@hero.getName}"
		initSitpositions

		puts @hero.getMen.length

		@hero.getMen.each{|man|
			puts man
			man.setMode("fight")
			startFightingMan(man)
		}
		puts "startFighting end"
	end
	
	def undefeatedMen
		return @men-getMenState("defeated")
	end
	
	def startFightingMan(man)
		return if not @target.getJob.is_a?(AntHeroFightJob)
		tmen=@target.getJob.undefeatedMen
		if tmen.length==0
			won
			return
		end
		# search nearest enemy
		dist=1000.0
		mtarget=nil
		tmen.each{|t|
			if t.getEnergy>0 and t.dead!=true
				d=(t.getPos2D-man.getPos2D).length
				if d<dist
					dist=d
					mtarget=t
				end
			end
		}
		if mtarget then
			if mtarget.getEnergy<=0
				raise "ERROR in startFightingMan"
			end
			man.newFightJob(0,mtarget)
			puts "#{man.getName} attacks #{mtarget.getName}"
		else
			man.newRestJob(30)
		end
	end

	def haveDefeated(e)
		puts "haveDefeated: #{e.getName}"
		@men.each{|m|
			puts "checking: #{m.fightTarget} #{e}"
			if m.fightTarget==e
				puts "#{m.getName} not longer attacks #{e.getName}"
				m.delJob # search other target
			end
		}
	end
	
	def checkFightMan(man)
		if man
			if not man.hasJob
				startFightingMan(man)
			end
		end
	end
	
	def checkFight(man)
		if not @target.getJob.is_a?(AntHeroFightJob)
			return
		end
		tmen=@target.getJob.undefeatedMen
		# not won yet, so go on
		checkFightMan(man)
		
		return false
	end
	def image
		"data/gui/sword.png"
	end

private
	def initSitpositions
		# gather sitting positions
		@sitpos={}
		@men.collect{|man|
			@sitpos[man]=@hero.getFormation(man,@hero.getPos2D)
		}
	end
end

class AntHeroFightAnimalJob<AntHeroMoveJob
	attr_reader :target
	def initialize(hero,target)
		@target=target
		super(hero,0,target.getPos2D,1,false)
		@finished=false
	end
	def image
		"data/gui/sword.png"
	end
	def check(man)
		if moveFinished
			case @state
				when "eat"
					man.newRestJob(10)
					@toEat-=1
					if @toEat<=0
						killAnimal
						playSound
						@finished=true
					end
				else
					@state="eat"
					@men.each{|m|
						m.newMoveJob(0,@target.getPos2D,0)
					}
					@toEat=@men.length
			end
		else
			super
		end
	end
	def playSound
		puts "FIXME: play eat sound"
	end
	def finished
		@finished
	end
	def killAnimal
		@target.eventDie
		@hero.resource.takeAll(@target.resource) #add("food",@target.resource.get("food"))
		#@target.resource.set("food",0)
	end
end

class AntHeroRecruitJob<AntHeroMoveJob
	attr_reader :finished
	def initialize(hero,target,agg)
		@target=target
		@aggression=agg
		@targetMen=target.menCount
		@want=@targetMen*agg/3
		@finished=false
		@restingMen=0
		@wantedMen=@want
		super(hero,0,target.getPos2D,4)
	end

	def image
		"data/gui/recruit.png"
	end

	def check(man)
		if moveFinished
			@hero.formation=AntFormationRest.new(@hero)
			if man.class==AntHero
				if (not @state=~/recruit/)
					if @want==0 then 
						@state="wait_recruit"
					else
						man.newRestJob(1)
						nman=@target.takeMan
						if nman.nil?
							@wantedMen=0
							return
						end
						nman.setBoss(@hero)
						nman.setMode("to_recruit")
						@want=@want-1

						# FIXME:
						getMen.each{|m|
							m.setMode("to_recruit")
						}
						@restingMen=0
						@wantedMen=@want+getMen.length-1

					end
				else
					super(man)
				end
			else
				case man.getMode
					when "to_recruit"
						fp=@hero.getFormation(man,@hero.getPos2D)
						man.newMoveJob(0,fp,0)
						man.setMode("recruit_torest")
					when "recruit_torest"
						man.newRestJob(1)
						man.setMode("recruit_resting")
						@restingMen+=1
						puts "RESTTEST: #{@restingMen} #{@wantedMen}"
						if @restingMen==@wantedMen
							@finished=true
						end
					when "recruit_resting"
						man.newRestJob(10)
				end
			end
		else
			super(man)
		end
	end
end


class AntHeroTakeJob<AntHeroMoveJob
	attr_reader :finished
	def initialize(hero,target,agg,what="food")
		super(hero,0,target.getPos2D,4)
		@what=what
		@target=target
		@aggression=agg
		@want=@men
		@oldpos=nil
		@takeStarted=false
	end

	def image
		if @what=="food"
			"data/gui/take_apple.png"
		else
			"data/gui/take_weapon.png"
		end
	end

	def check(man)
		if moveFinished
			if man.class==AntHero	and @oldpos.class==NilClass
				@oldpos=man.getPos2D
				if @takeStarted==false
					@men.each{|m|m.delJob}
					@takeStarted=true
				end
			end
			case man.getMode
				when "takingFood"
					# take food and return
					case @what
						when "food"
							for i in 1..@hero.getAggression
								if @target.resource.get("food")>0
									@target.resource.sub("food",1)
									man.resource.add("food",1)
								end
							end
						when "weapon"
							["sword","bow","boat","shield"].each{|w|
								if @target.resource.get(w)>0 and man.resource.get(w)==0 # take only if house has it and man doesn't have it
									@target.resource.sub(w,1)
									man.resource.add(w,1)
								end
							}
						else
							raise "unknown type to take !"
					end
					@want.delete(man)
					if @want.length==0
						@finished=true
						@hero.eventHLJobFinished(self)
						@hero.newHLMoveJob(0,@oldpos,0)
					end
				else
					man.newMoveJob(0,@target.getPos2D,0)
					man.setMode("takingFood")
			end
		else
			super(man)
		end
	end
end


class ProductionRule
	attr_reader :what, :from
	def initialize(what,from)
		@what=what
		@from=from
	end
end

$productionRules=[
	ProductionRule.new("rod",[["wood",1]]),
	ProductionRule.new("steel",[["ore",1],["coal",1]]),
	ProductionRule.new("gold",[["ore",4],["coal",1]]),
	ProductionRule.new("boat",[["wood",2]]),
	ProductionRule.new("shield",[["wood",1]]),
	ProductionRule.new("bow",[["wood",1],["stone",1]]),
	ProductionRule.new("sword",[["wood",1],["steel",1]])
]

# constructing runs like this:
# * depending on aggression some count of men is used for constructing
# * those men run to "target" and work there
# * when finished they leave tools/weapons/anything there and come back to hero to rest a little
# * job finishes after some given time (20 seconds or so)

class AntHeroConstructJob<AntHeroMoveJob
	attr_reader :finished
	def initialize(hero,target,agg)
		super(hero,0,target.getPos2D,4)
		@target=target
		@usedmen=0
		@restype={}
		@constructStarted=false
		@productionRules=$productionRules # FIXME maybe exchange for different constructing types
	end
	def image
		"data/gui/construct.png"
	end
	def check(man)
		if moveFinished
			@hero.formation=AntFormationRest.new(@hero)
			if man.is_a?(AntBoss)
				checkEat(man)
				if not @constructStarted
					@men.each{|m|m.delJob}
					@constructStarted=true
				end
				man.newRestJob(20) # do nothing at all
				return
			end
			@constructStarted=true
			wantmen=(@men.length-1)*@hero.getAggression/3.0
			case man.getMode
				when "fetch" # go to resource
					res=getNeededResources
					res,nearest=getNextWithResource(res)
					@restype[man]=[res,nearest]
					if nearest.nil?
						puts "resource not found! #{res}"
						man.newRestJob(5)
					else
						if (nearest.getPos2D-@target.getPos2D).length<20
							man.newMoveJob(0,nearest.getPos2D,0.5) # near but not actually there
							man.setMode("harvest")
						else
							man.newRestJob(1)
							puts "TOO FAR"
						end
					end
				when "harvest" # harvest resource
					if (@restype[man][1].getPos2D-man.getPos2D).length<3
						man.newRestJob(2)
						man.digResource(@restype[man][0])
						man.setMode("collect")
					else
						puts "WAS TOO FAR AWAY - Probably job discarded"
						man.newMoveJob(0,@target.getPos2D,0)
						man.setMode("")
					end
				when "collect" # bring back
					# FIXME: add sub resource from nearest
					p=@restype[man]
					amount=[p[1].resource.get(p[0]),man.canCarry].min
					p[1].resource.sub(p[0],amount)
					man.resource.add(p[0],amount)
					man.newMoveJob(0,@target.getPos2D,0)
					man.collectResource(@restype[man][0])
					man.setMode("brought")
				when "brought"
					man.setMode("after_brought")
					man.newRestJob(1)
					# take all natural resources - but not food and weapons - otherwise men starve!
					myResources.each{|r|
						@target.resource.take(man.resource,r)
					}
				when "after_brought"
					fpos=@hero.getFormation(man,@hero.getPos2D)
					man.newMoveJob(0,fpos,0)
					man.setMode("construct_torest")
				when "to_construct"  # do some constructing
					@target.incSmoke
					man.newRestJob(5 - @hero.getAggression*0.5) # work for 3.5-5 seconds (depending on aggression)
					man.setMode("constructing")
					man.playSound("construct")
				when "constructing"
					# was constructing
					readyConstructed
					man.incExperience(man.learnAmount)
					@target.decSmoke
					man.setMode("construct_torest")
					fpos=@hero.getFormation(man,@hero.getPos2D)
					man.newMoveJob(0,fpos,0)
				when "construct_torest"
					man.setMode("rest")
					man.newRestJob(3/@hero.getAggression) # shorter pauses when aggression is higher
					if man.getAggression<3
						checkEat(man)
					end
					@usedmen-=1
				else
					if wantmen>@usedmen
						@usedmen+=1
						man.newMoveJob(0,@target.getPos2D,0)
						if enoughResources
							man.setMode("to_construct")
						else
							man.setMode("fetch")
						end
					end
			end
		else
			super(man)
		end
	end
private
	def myResources
		["wood","stone","coal","ore"]
	end
	def enoughResources
		# FIXME support more resources
		m=myResources.min{|a,b|@target.resource.get(a)<=>@target.resource.get(b)}
		value=@target.resource.get(m)
		return value>5
		#(@target.resource.get("stone")>5 and @target.resource.get("wood")>5)
	end
	def getNeededResources
		m=myResources.sort{|a,b|@target.resource.get(a)<=>@target.resource.get(b)}
		puts "NEEDED: #{m}"
		return m
	end
	def getNextWithResource(res)
		#goods={"wood"=>"tree","stone"=>"stone","food"=>"tree","coal"=>"mine","ore"=>"mine"}
		#enttype=goods[res]
		ret=nil
		res.each{|r|
			ret=getMap.getNext(@target,r,1)
			if not ret.nil?
				ret=[r,ret]
				break
			end
		}

		ret
	end
	def checkEat(man)
		puts "CHECKEAT"
		if man.getFood<0.5
			if man.resource.get("food")>0
				man.incFood(1)
				man.resource.sub("food",1)
			elsif heroHasFood
				man.incFood(1)
				@hero.resource.sub("food",1)
			end
		end
	end
	def heroHasFood
		@hero.resource.get("food")>0
	end
	def readyConstructed
		# produce any
		@productionRules.shuffle.each{|rule|
			ok=true
			
			rule.from.each{|w,n|
				puts "#{w},#{n}"
				if @target.resource.get(w)<n
					ok=false
				end
			}
			if ok
				# we found a useful production
				rule.from.each{|w,n|@target.resource.sub(w,n)}
				@target.resource.add(rule.what,1)
				return #out
			end
		}
		# something went wrong - we don't have enough resources - whatever
	end
end


class AntHeroRestJob<AntHLJob
	def initialize(hero,time)
		@hero=hero
		@time=time
		@hero.newRestJob(time)
		@spreadThings=false
		@men=hero.getMen
		@hero.formation=AntFormationRest.new(@hero)
		@basePos=@hero.getPos2D
		@firstTime=true
	end
	def makeMessage(boss)
		RestMessage.new(boss,@time)
	end
	def image
		"data/gui/bed.png"
	end
	def check(man)
		if man.is_a?(AntHero)
			if @firstTime
				man.newRestJob(@time)
				@firstTime=false
			end
			eat(man)
			#spreadThings
			return
		end
		case man.getMode
			when "rest_eat"
				spreadThings
				eat(man)
				sit(man)
				man.setMode("rest_sit")
			when "rest_sit"
				sit(man)
				if man.getFood<0.5 and heroHasFood
					man.newMoveJob(0,@hero.getPos2D,0)
					man.setMode("rest_eat")
				end
			else
				# rest_eat
				man.newMoveJob(0,@hero.getPos2D,0)
				man.setMode("rest_eat")
		end
	end
	def finished
		return (not @hero.hasJob)
	end
private
	def sit(man)
 		formationPos=@hero.getFormation(man,@basePos)

		diff=(man.getPos2D-formationPos)
		dist=diff.length2
		if dist<0.1 then
			man.setDirection(180-(@hero.getPos2D-man.getPos2D).normalized.getAngle.angle*180.0/Math::PI)
			if man.getPos3D.z<0 # under water
				man.newRestJob(5)
				man.setMeshState("stand")
			elsif not ["sitdown","sit"].member?(man.meshState)
				man.sitDown
			else
				man.newRestJob(5)
				man.setMeshState("sit")
			end
		else
			puts "#{man.getPos2D} #{formationPos} #{dist} #{diff}"
			man.newMoveJob(0,formationPos,0)
		end
	end
	def eat(man)
		if man.getFood<0.5
			if man.resource.get("food")>0
				man.incFood(1)
				man.resource.sub("food",1)
			elsif heroHasFood
				man.incFood(1)
				@hero.resource.sub("food",1)
			end
		end
	end
	def heroHasFood
		@hero.resource.get("food")>0
	end
	def spreadThings
		if @spreadThings==false
			@spreadThings=true
			all={}
			rs=["sword","shield","bow","boat"]
			rs.each{|r|	
				all=0
				@men.each{|m|
					c=m.resource.get(r)
					all+=c
				}
				if all>=@men.length
					# give everyone 1 of these and put the rest on the hero
					@men.each{|m|
						m.resource.set(r,1)
					}
					@hero.resource.add(r,all-@men.length)
				else
					men=@men.sort {|b,a|a.resource.get(r)<=>b.resource.get(r)} # sort descending
					# put hero at first
					men.delete(@hero)
					men=[@hero]+men
					men.each{|m|m.resource.set(r,0)} # reset
					for i in 1..all # now give to those who had a weapon and to hero (at first)
						men[i-1].resource.set(r,1)
					end
				end
			}
			# spread food equally
			food=0
			@men.each{|m|food+=m.resource.get("food")}
			min=(food/@men.length).to_i
			@men.each{|m|m.resource.set("food",min)}
			# spread rest on first
			food-=min*@men.length
			for i in 1..food
				@men[i-1].resource.add("food",1)
			end
		end
	end
end

class AntHouseFetchJob
	def initialize(house)
		@house=house
	end
	def check
	end
end


require 'ant_buildjob.rb'