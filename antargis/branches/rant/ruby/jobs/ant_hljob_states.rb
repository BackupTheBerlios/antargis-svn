# Does the walk-formation
# 
#
require 'jobs/ant_state_machine.rb'

class Module
	def wrap(objectName,methodName,objectMethodName=nil)
		objectMethodName||=methodName
		ts="*s"
		ts="s" if methodName=~/.*=$/
		s="def #{methodName}(#{ts})\n#{objectName}.#{objectMethodName}(#{ts})\nend\n"
		puts s
		module_eval s
	end
end


module HLJob_Additions
	attr_accessor :machine
	["hero","allMen","getTime","targetPos","targetPos=","formatDir","formatDir=","target"].each{|n|wrap "machine",n}
end

class HLJob_BaseState
	include HLJob_Additions

	def trace
		if @hero.nil?
			puts "TRACE #{caller[0]} #{self}"
		else
			puts "TRACE #{caller[0]} #{self} #{hero} #{getTime}"
		end
	end
end

class HLJob_DummyState<HLJob_BaseState
end

class HLJob_FormatWalk<HLJob_BaseState
	# needed: getTime

	# wait 5 seconds at max for formatting
	FORMAT_MAX_TIME=5

	def enter
		puts "#{self}:enter"
		trace
		hero.formation=AntFormationBlock.new(hero,formatDir)
		heroPos=hero.getPos2D
		allMen.each{|man|
			pos=hero.getFormation(man,heroPos)
			man.walkTo(pos)
			man.hlJobMode[:formatting]=true
		}
		@formatStart=getTime
	end

	def assign(man)
		#raise 1
		man.setDirection(180-(targetPos-hero.getPos2D).normalized.getAngle.angle*180.0/Math::PI)
		man.standStill
		man.hlJobMode.delete(:formatting)
	end

	def ready
		if getTime-@formatStart>FORMAT_MAX_TIME
			puts "MUST BE READY"
			return true
		end
		allMen.each{|man|
			puts "formatting:#{man}:#{man.hlJobMode[:formatting]}"
			if man.hlJobMode[:formatting]
				return false
			end
		}
		return true
	end

private

end

class HLJob_FormatSit<HLJob_BaseState
	# needed: getTime

	# wait 5 seconds at max for formatting
	FORMAT_MAX_TIME=15

	def enter
		#raise 1
		trace
		hero.formation=AntFormationRest.new(hero)
		heroPos=hero.getPos2D
		allMen.each{|man|
			pos=hero.getFormation(man,heroPos)
			man.walkTo(pos)
			man.hlJobMode[:formatting]=true
		}
		@formatStart=getTime
	end

	def assign(man)
		man.setDirection(180-(hero.getPos2D-man.getPos2D).normalized.getAngle.angle*180.0/Math::PI)
		man.standStill
		man.hlJobMode.delete(:formatting)
	end

	def ready
		puts "#{self}:ready"
		if getTime-@formatStart>FORMAT_MAX_TIME
			# FIXME:rest deserts
			return true
		end
		allMen.each{|man|
			if man.hlJobMode[:formatting]
				return false
			end
		}
		return true
	end
end

class HLJob_MoveToNextWayPoint<HLJob_BaseState
	# needed: hero,allMen,targetPos, near

	attr_accessor :near

	def initialize
		super
		@near=0
	end

	def enter
		puts "#{self}:enter"
		hero.formation=AntFormationBlock.new(hero,formatDir)
		allMen.each{|man|
			pos=hero.getFormation(man,targetPos)
			man.newMoveJob(0,pos,near)
			man.hlJobMode[:walking]=true
		}
	end

	def assign(man)
		man.standStill
		man.hlJobMode.delete(:walking)

		# FIXME: maybe let him desert if this is called too often ?
	end

	def ready
		allMen.each{|man|
			if man.hlJobMode[:walking]
				return false
			end
		}
		return true
	end

end

class HLJob_MoveComplete<BaseState
	include HLJob_Additions
	state :moveToNextPoint=>HLJob_MoveToNextWayPoint
	state :formatWalk=>HLJob_FormatWalk
	state :endState=>HLJob_DummyState

	startState :formatWalk
	endState :endState
	
	edge :moveToNextPoint, :formatWalk, :stillHasWaypoints
	edge :formatWalk, :moveToNextPoint
	edge :moveToNextPoint, :endState, :noMoreWaypoints

	def enter
		puts "#{self}:enter"
		if @waypoints.nil?
			initWaypoints
		end
	end

	def stillHasWaypoints
		puts state
		if @waypoints.length>0
			self.targetPos=@waypoints.shift
			puts "targetPos:#{targetPos}"
			return true
		end
		false
	end

	def near=(n)
		@states[:moveToNextPoint].near=n
	end

	def noMoreWaypoints
		@waypoints.length==0
	end

	def moveDirectly
		@state=:moveToNextPoint
	end

	def assign(man)
		#raise 1
		state.assign(man)
	end

	def hero
		machine.hero
	end
	def formatDir
		(targetPos-hero.getPos2D).normalized
	end

	private

	def initWaypoints
		if getMap.path
			@waypoints=[hero.getPos2D]+getMap.path.computePath(hero.getPos2D,targetPos,hero)+[targetPos]
			# remove waypoints in between - if they're not necessary - origin must be given, too
			@waypoints=getMap.path.refinePath(@waypoints,hero)
			# remove origin for waypoint-list
			assert{@waypoints.length>=2}
			@waypoints.shift
			
			@waypoints+=[targetPos]

		else
			@waypoints=[targetPos]
		end
		@completeTargetPos=targetPos
		self.targetPos=@waypoints.shift
	end
end

class HLJob_SitDown<HLJob_BaseState
	def enter
		allMen.each{|man|
			man.setDirection(180-(hero.getPos2D-man.getPos2D).normalized.getAngle.angle*180.0/Math::PI)
			man.sitDown
			man.hlJobMode[:sitting]=true
		}
	end

	def assign(man)
		man.sitStill
		man.hlJobMode.delete(:sitting)
	end

	def ready
		allMen.each{|man|
			if man.hlJobMode[:sitting]
				return false
			end
		}
		return true
	end
end

class HLJob_JustSit<HLJob_BaseState
	def enter
		allMen.each{|man|
			man.sitStill
		}
	end
	def assign(man)
		man.sitStill # and rest
	end

	def ready
		return false
	end
end

class HLJob_JustSitOnce<HLJob_BaseState
	def enter
		allMen.each{|man|
			man.sitStill
			man.hlJobMode[:justSitting]=true
		}
	end
	def assign(man)
		man.hlJobMode.delete(:justSitting)
	end
	def ready
		allMen.each{|man|
			if man.hlJobMode[:justSitting]
				return false
			else
				return true
			end
		}
	end
end
	

class HLJob_FetchStart<HLJob_BaseState
	# needed a target-entity
	def enter
		trace
		fetchPoint=machine.target.getPos2D
		puts "fetchPoint:#{fetchPoint}  heroPos:#{hero.getPos2D}"
		allMen.each{|man|
			man.walkTo(fetchPoint)
			man.hlJobMode[:fetching]=true
			puts "set to fetching: #{man}"
		}
		
	end

	def assign(man)
		trace
		man.standStill
		man.hlJobMode.delete(:fetching)
	end

	def ready
		trace
		allMen.each{|man|
			puts "fetchStart_ready check: #{man} : #{man.hlJobMode[:fetching]}"
			if man.hlJobMode[:fetching]
				return false
			end
			puts "false"
		}
		puts "return true ??"
		return true
	end
end

class HLJob_GetResource<HLJob_BaseState
	# needed: resources return array of resource-strings
	attr_accessor :resources

	def initialize
		@resources=[]
	end

	def enter
		for i in 1..hero.getAggression
			allMen.each{|man|
				resources.each{|r|
					a=machine.target.resource.get(r)
					if a>1
						man.resource.add(r,1)
						target.resource.sub(r,1)
					end
				}
			}
		end
		allMen.each{|m|m.resourceChanged}
	end
	def ready
		return true
	end
end

class HLJob_KillAnimal<HLJob_BaseState
	def enter
		target.eventDie
		hero.resource.takeAll(target.resource)
		allMen.each{|m|m.resourceChanged}
	end
end

# spread things among team-members
class HLJob_SpreadThings<HLJob_BaseState
	RESOURCES_TO_SPREAD=["sword","shield","bow","boat"]

	def enter
		allMen.each{|man|
			man.walkTo(hero.getPos2D)
			man.hlJobMode[:gatherToSpread]=true
		}
		@alreadySpread=false
	end

	def assign(man)
		man.standStillShort
		if man.hlJobMode[:gatherToSpread]
			man.hlJobMode.delete(:gatherToSpread)
			man.hlJobMode[:waitForSpread]=true
		elsif man.hlJobMode[:waitForSpread]
			man.hlJobMode.delete(:waitForSpread)
		else
			# nothing for now
		end
	end

	def ready
		return true if @alreadySpread		
		allMen.each{|man|
			return false if man.hlJobMode[:gatherToSpread] or man.hlJobMode[:waitForSpread]
		}
		# we can spread now
		doSpreading
		@alreadySpread=true
		return true
	end
	private
	
	def doSpreading
		# FIXME: maybe this can be done more easily ???
		trace
	
		all={}
		RESOURCES_TO_SPREAD.each{|r|	
			all=0
			allMen.each{|m|
				c=m.resource.get(r)
				all+=c
			}
			if all>=allMen.length
				# give everyone 1 of these and put the rest on the hero
				allMen.each{|m|
					m.resource.set(r,1)
				}
				hero.resource.add(r,all-allMen.length)
			else
				men=allMen.sort {|b,a|a.resource.get(r)<=>b.resource.get(r)} # sort descending
				# put hero at first
				men.delete(hero)
				men=[hero]+men
				men.each{|m|m.resource.set(r,0)} # reset
				for i in 1..all # now give to those who had a weapon and to hero (at first)
					men[i-1].resource.set(r,1)
				end
			end
		}
		# spread food equally
		food=0
		allMen.each{|m|food+=m.resource.get("food")}
		min=(food/allMen.length).to_i
		allMen.each{|m|m.resource.set("food",min)}
		# spread rest on first
		food-=min*allMen.length
		for i in 1..food
			allMen[i-1].resource.add("food",1)
		end

		allMen.each{|m|m.resourceChanged}
	end
	
end

class HLJob_FightData<HLJob_BaseState

	attr_reader :inited
	# only attacker creates fightData
	def initialize(fightJob)
		@parties={:attacker=>[fightJob],:defender=>[]}
		@oldparties=@parties.dup
	end
	def add(fightJob)
		@parties[getFightType(fightJob)].push(fightJob)
		@oldparties[getFightType(fightJob)].push(fightJob)
		reshuffle
	end

	# this called if (and only if) the hero has won or is fleeing (and thus aborting the fight)
	def remove(fightJob)
		@parties[fightJob.fightType].delete(fightJob)
		@oldparties[fightJob.fightType].delete(fightJob)
		reshuffle
	end

	# this called if (and only if) the hero has lost
	def removeLost(fightJob)
		@parties[fightJob.fightType].delete(fightJob)
		reshuffle
	end

	def getFightType(fightJob)
		target=fightJob.target
		[:attacker,:defender].each{|t| 
			return otherType(t) if @parties[t].collect{|a|a.hero}.member?(target)
		}
		raise "Target #{fightJob} (target:#{target}) not found in getFightType!"
	end

	def getNewOpponent(man,fightJob)
		assert{man.canFight}
		assert{man.is_a?(AntHero) or man.is_a?(AntMan)}
		assert{man.getHero==fightJob.hero}
	
		checkForUpdate

		# for a start: get next opponent
		# (maybe take next free opponent ??)
		# (maybe: find weakest friend and help him ???)

		myType=fightJob.fightType
		oType=otherType(myType)

		opponent=@parties[oType].collect{|job|job.undefeatedMen}.flatten.uniq.shuffle[0]
		if opponent.nil?
			# we won ???
			#eventWon(myType)
			return nil
		else
			assert{opponent.canFight}
			return opponent
		end
	end

	def checkForUpdate

		#raise "FIXME"
		# check, if heroes are still able to fight, else reshuffle - let hero lose or sth else

		nparties={}
		oldparties=@parties
		@parties.each{|type,jobs|
			njobs=jobs.select{|job|job.hero.canFight}
			nparties[type]=njobs
		}
		@parties=nparties

		if @parties!=oldparties
			reshuffle
		end

	end

	private

	def otherType(my)
		{:attacker=>:defender,:defender=>:attacker}[my]
	end

	def getAllJobs
		@parties.collect{|type,jobs|jobs}.flatten
	end
	def getAllHeroes
		getAllJobs.collect{|job|job.hero}
	end

	def reshuffle
		trace
		# reinit and assign

		menGroup={}
		leave=false
		@parties.each{|type,jobs|
			puts "parties: #{type}:#{jobs}"
			menGroup[type]=jobs.collect{|job|job.undefeatedMen}.flatten.uniq
			menGroup[type].each{|man|man.delJob}
			leave=true if @inited and menGroup[type].length==0 # a fightjob is leaving
		}
		return if leave
	
		# check that each group has more than 0 members
		assert{menGroup.select{|k,v|v.length==0}.length==0}

		defenders=menGroup[:defender].dup

		defenders.each{|d|d.hlJobMode.delete(:fightTarget)}
		
		assert{defenders.length>0}

		# each attacker gets an opponent - if all defenders are used - restart them agaim
		menGroup[:attacker].each{|attacker|
			defender=defenders.min{|a,b|(a.getPos2D-attacker.getPos2D).length<=>(b.getPos2D-attacker.getPos2D).length}

			assert{not defender.nil?}

			attacker.hlJobMode[:fightTarget]=defender
			if not defender.hlJobMode.member?(:fightTarget)
				defender.hlJobMode[:fightTarget]=attacker
			end

			defenders.delete(defender)

			defenders=menGroup[:defender].dup if defenders.length==0
		}
		menGroup[:defender].each{|defender|
			if not defender.hlJobMode.member?(:fightTarget)
				# assign remaining defenders to attackers
				attacker=menGroup[:attacker].min{|a,b|(a.getPos2D-defender.getPos2D).length<=>(b.getPos2D-defender.getPos2D).length}
				defender.hlJobMode[:fightTarget]=attacker
			end
		}
		@inited=true
	end


end

class HLJob_Fight<HLJob_BaseState
	# needed: hero,target, undefeatedMen

	attr_reader :fightType, :fightData, :won

	def enter
		@oldHeroPosition=hero.getPos2D
		trace

		targetHadFightData=target.hlJobMode[:fightData].nil?
		
		puts "fightData: #{target.hlJobMode[:fightData]}"
		checkForFightData
		if targetHadFightData
			puts "NOT YET INITED #{self} hero:#{hero} target:#{target}"
			target.newHLDefendJob(hero)
		end
		trace
		assignAllJobs
	end

	def assign(man)
		if man.is_a?(AntHouse)
			man.newRestJob(20)
			return
		end

		if not man.canFight	
			# it's possible that man is defeated is the same time that another is defeated ?
			# this way his morale is low - but eventDefeated wasn't called yet
			man.hlJobMode[:defeated]=true
		end

		if man.hlJobMode[:defeated]
			# FIXME:send away
			# FIXME: maybe flee ?
			if hero.is_a?(AntHouse)
				if man.hlJobMode[:homing]
					man.standStill
				else
					man.hlJobMode[:homing]
					man.walkTo(@oldHeroPosition+AGVector2.new(getRand*2-1,getRand*2-1))
				end
			else
				# FIXME
			end
			man.hlJobMode[:defeated]=true
			man.hlJobMode.delete(:fighting)
		else
			puts ".canFight:#{man.canFight} #{man.getEnergy} #{man.getMorale}"
			assert{man.canFight}

			opponent=@fightData.getNewOpponent(man,self)
			if opponent.nil?
				# won
				@won=true
			else
				assert{man.canFight and opponent.canFight}
				man.hlJobMode[:fightTarget]=opponent
				man.newFightJob(0,man.hlJobMode[:fightTarget])
			end
		end
	end

	def leave
		if lost
			@fightData.removeLost(self)
		else
			@fightData.remove(self)
		end
		@fightData=nil
		@fightType=nil
		hero.hlJobMode.delete(:fightType)
		hero.hlJobMode.delete(:fightData)

		allMen.each{|man|
			man.hlJobMode.delete(:defeated)
			man.hlJobMode.delete(:fighting)
			man.hlJobMode.delete(:homing)
		}
	end

	def ready
		return true if @ready
		if won 
			@won=true
			machine.eventWon(target)
			@ready=true
		elsif lost
			@lost=true
			machine.eventLost(target)
			@ready=true
		end
			
		return @ready
	end

	def lost
		((not @won) and undefeatedMen.length==0) or (not hero.canFight)
	end

	def undefeatedMen
		allMen.select{|man|man.hlJobMode[:defeated].nil? && man.canFight}
	end

	private
	# return true if fightData already exists
	def checkForFightData
		trace
		puts "TARGET #{target}"
		if target.hlJobMode[:fightData]
			trace
			@fightData=target.hlJobMode[:fightData]
			@fightData.add(self)
			@fightType=@fightData.getFightType(self)
			return true
		else
			trace
			@fightData=HLJob_FightData.new(self)
			@fightType=:attacker
		end
		puts "assigned fightData!"
		hero.hlJobMode[:fightData]=@fightData # store so that it's avaiable above
	end
	def assignAllJobs
		# FIXME:assign a fight-job to every member (undefeatedMen)
		trace

		undefeatedMen.each{|man|
			assert{not man.hlJobMode[:fightTarget].nil?}
			man.newFightJob(0,man.hlJobMode[:fightTarget])
			# FIXME: check if morale is high enough
			man.hlJobMode.delete(:defeated)
			man.hlJobMode[:fighting]=true
		}
	end
end


class HLJob_Recruit<HLJob_BaseState
	def enter
	end
	
	private
	def recruitGetMen
		
	end
end

