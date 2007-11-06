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
		#puts s
		module_eval s
	end
end


module HLJob_Additions
	attr_accessor :machine
	["getMap","getRand","hero","allMen","getTime","targetPos","targetPos=","formatDir","formatDir=","target"].each{|n|wrap "machine",n}
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
			#log "MUST BE READY"
			return true
		end
		allMen.each{|man|
			#log "formatting:#{man}:#{man.hlJobMode[:formatting]}"
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
		@near=0
		puts "#{self}:enter"
		if @waypoints.nil?
			initWaypoints
		end
	end

	def stillHasWaypoints
		puts state
		if @waypoints.length>0
			self.targetPos=@waypoints.shift

			if @waypoints.length==0
				self.targetPos=checkPosNear(self.targetPos)
				@states[:moveToNextPoint].near=@near 
			end
			
			return true
		end
		false
	end

	def near=(n)
		@near=n
		#@states[:moveToNextPoint].near=n
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
		@finalPos=targetPos
		if getMap.path
			@waypoints=[hero.getPos2D]+getMap.path.computePath(hero.getPos2D,targetPos,hero)+[targetPos]
			# remove waypoints in between - if they're not necessary - origin must be given, too
			@waypoints=getMap.path.refinePath(@waypoints,hero)
			# remove origin for waypoint-list
			assert{@waypoints.length>=2}
			@waypoints.shift
			
			#@waypoints+=[targetPos]

		else
			@waypoints=[targetPos]
		end
		@completeTargetPos=targetPos
		self.targetPos=@waypoints.shift
		self.targetPos=checkPosNear(self.targetPos)
	end

	def checkPosNear(to)
		from=hero.getPos2D
		line=AGLine2.new(from,to)
		circle=AGCircle2.new(@finalPos,@near)
		list=circle.collide(line)
		if list.length>0
			to=list.min{|a,b|(a-from).length<=>(b-from).length}
		end

		to
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
		machine.target.resourceChanged
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
		target.resourceChanged
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
			if menGroup[type].length==0
				if @inited
					leave=true # a fightjob is leaving
				else
					# party is still defeated, but job is not yet finished ?
					leave=true 
				end
			end
		}
		return if leave
		assert{@parties.length>0}
	
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
			if man.hlJobMode[:fightTarget]
				assert{not man.hlJobMode[:fightTarget].nil?}
				man.newFightJob(0,man.hlJobMode[:fightTarget])
				# FIXME: check if morale is high enough
				man.hlJobMode.delete(:defeated)
				man.hlJobMode[:fighting]=true
			else
				# this the case, when an already defeated party is attacked
				puts "POSSIBLE ERROR: #{man} has no opponent ?"
			end
		}
	end
end


#
# Recruiting goes like follows:
# 
#
class HLJob_Recruit<HLJob_BaseState
	def enter
		@countTargetMen=target.getMen.length
		@countRecruiting=0
		@countRecruited=0
		@myPos=hero.getPos2D
		initRecruiting
	end

	def ready
		((@countRecruited>=howManyToRecruit) or nonToRecruitLeft) and (hero.getPos2D-hero.getFormation(hero,@myPos)).length<0.1
	end

	def assign(man)
		if checkRecruited(man)
			returnToStart(man)
			return
		end

		if (not ready)
			if not letRecruit(man)
				returnToStart(man)
			end
		else
			returnToStart(man)
		end
	end

	private

	def howManyToRecruit
		@countTargetMen*hero.getAggression/3
	end

	def checkRecruited(man)
		if man.hlJobMode[:recruitTarget]
			target=man.hlJobMode[:recruitTarget]
			target.setBoss(hero)
			target.hlJobMode[:recruitTarget]=nil
			man.hlJobMode[:recruitTarget]=nil
			returnToStart(target)
			@countRecruited+=1
			return true
		end
		return false
	end

	def returnToStart(man)
		pos=hero.getFormation(man,@myPos)
		if (man.getPos2D-pos).length<0.3
			man.lookTo(@myPos)
			man.standStill
			puts "standStill #{man}"
		else
			man.walkTo(pos)
			puts "walkTo #{pos} #{man}"
		end
	end

	def getAssignableTargets
		target.getMen-hero.getMen.map{|man|man.getTarget}		
	end

	def nonToRecruitLeft
		getAssignableTargets==0
	end

	def getNext(man,targets)
		targets.min{|a,b|(a.getPos2D-man.getPos2D).length<=>(b.getPos2D-man.getPos2D).length}
	end

	def letRecruit(man)
		target=getNext(man,getAssignableTargets)
		if target
			man.hlJobMode[:recruitTarget]=target
			man.newMoveJob(0,target,1)
			@countRecruiting=@countRecruiting+1
			return true
		end
		false
	end

	def initRecruiting
		hero.getMen.each{|man|man.hlJobMode[:recruitTarget]=nil}
		# hero at last
		hero.getMen.reverse.each{|man|
			letRecruit(man)
			break if @countRecruiting>=howManyToRecruit
		}
	end
end

begin
	class ProductionRule
		attr_reader :what, :from
		def initialize(what,from)
			@what=what
			@from=from
		end
	end
rescue;end

class ConstructException<Exception
end


module HLJob_HarvestModule
	# :section: harvesting
protected
	def goHarvesting(man)
		list=whatToHarvestList
		list.each{|what|
			#pp what
			assert{what.is_a?(String)}
			# check if 'what' is reachable
			entity=getNextWithResource(what)
			if entity
				man.hlJobMode[:task]=:harvest
				man.hlJobMode[:what]=what
				man.hlJobMode[:target]=entity
				man.walkTo(entity)
				break
			end
		}
	end

	def harvest(man)
		error("wrong fetch-resource") unless resources.member?(man.hlJobMode[:what])
		error("man has invalid target") if man.hlJobMode[:target].nil?
		error("target doesn't have resource (#{man.hlJobMode[:what]})") unless man.hlJobMode[:target].resource.get(man.hlJobMode[:what])>0
		error("target is too far away") unless (man.hlJobMode[:target].getPos2D-man.getPos2D).length<1

		man.newRestJob(4,true)
		man.digResource(man.hlJobMode[:what])
		man.hlJobMode[:task]=:comeBack
	end

	def comeBack(man)
		ctarget=man.hlJobMode[:target]
		what=man.hlJobMode[:what]
		error("target is not ok") unless ctarget.is_a?(AntEntity)
		error("selected resource is not ok") unless what.is_a?(String)
		error("target doesn't have enough resource") unless ctarget.resource.get(what)>0
		# ok, finished harvesting - take what's harvested
		
		amount=[ctarget.resource.get(what),man.canCarry].min
		ctarget.resource.sub(what,amount)
		man.resource.add(what,amount)
		man.newMoveJob(0,target.getPos2D,0)
		man.collectResource(what)

		# come back
		man.hlJobMode[:task]=:deliverAll
		log "#{man}(#{man.getPos2D}) coming back to #{target}(#{target.getPos2D}) from #{ctarget}(#{ctarget.getPos2D})"
		log "job of #{man}:#{man.getJobName}"
		man.hlJobMode[:target]=nil
	end

	def deliverAll(man)
		error("not yet at target") if (target.getPos2D-man.getPos2D).length>1
		resources.each{|res|
			amount=man.resource.get(res)
			target.resource.add(res,amount)
			man.resource.sub(res,amount)
		}
		man.resourceChanged
		target.resourceChanged
		man.newRestJob(2)
		man.hlJobMode[:task]=nil
		man.hlJobMode[:done]+=1
		man.hlJobMode[:what]=nil
	end

end

module HLJob_RestingModule
	# :section: Resting

	def goResting(man)
		man.walkTo(hero.getFormation(man,hero.getPos2D))
		man.hlJobMode[:task]=:sitDown
	end
	def sitDown(man)
		man.sitDown
		man.hlJobMode[:task]=:rest
		man.lookTo(hero.getPos2D)
	end
	def rest(man)
		man.sitStill
		man.hlJobMode[:task]=nil
		man.hlJobMode[:done]=0
		man.lookTo(hero.getPos2D)
	end
end



class HLJob_CreationBase<HLJob_BaseState
	def enter
		hero.getMen.each{|man| man.hlJobMode.clear ; man.hlJobMode[:done]=0}
		hero.getMen.each{|man|assign(man)}
	end

	def leave
		hero.getMen.each{|man|target.decSmoke if man.hlJobMode[:task]==:producing}
	end

	def assign(man)
		if man.is_a?(AntHero)
			man.sitStill
			return
		end

		#pp man.hlJobMode
		begin
			if man.hlJobMode[:task].nil?
				man.hlJobMode[:task]=whatToDo(man)
			end
			self.send(man.hlJobMode[:task],man)
		rescue ConstructException => e
			log "don't know what to do for #{man} : exceptino #{e}"
			man.hlJobMode.clear
			assign(man)
		end
	end

end


class HLJob_Construct<HLJob_CreationBase
	PRODUCTION_RULES=[
		ProductionRule.new("rod",[["wood",1]]),
		ProductionRule.new("steel",[["ore",1],["coal",1]]),
		ProductionRule.new("gold",[["ore",4],["coal",1]]),
		ProductionRule.new("boat",[["wood",2]]),
		ProductionRule.new("shield",[["wood",1]]),
		ProductionRule.new("bow",[["wood",1],["stone",1]]),
		ProductionRule.new("sword",[["wood",1],["steel",1]])
	]
	RESOURCES=PRODUCTION_RULES.map{|rule|rule.from.map{|f|f[0]}}.flatten.uniq-PRODUCTION_RULES.map{|rule|rule.what}
	TARGETS=PRODUCTION_RULES.map{|rule|rule.what}.uniq

	include HLJob_HarvestModule
	include HLJob_RestingModule

	VALID_STATES=[
		:goHarvesting,:harvest,:comeBack,:deliverAll,
		:goProducing,:produce,:endProduce,
		:goResting,:sitDown,:rest
	]

	def resources
		RESOURCES
	end


	def ready
		TARGETS.each{|t|return false if target.resource.get(t)<constructAtMost}
		true
	end

	private

	# :section: Producing

	def goProducing(man)
		man.walkTo(target)
		man.hlJobMode[:task]=:produce
	end

	def produce(man)
		target.incSmoke
		man.hlJobMode[:task]=:endProduce
		man.newRestJob(3,true)
	end

	def endProduce(man)
		target.decSmoke
		man.hlJobMode[:task]=nil

		# decide whats to produce
		PRODUCTION_RULES.shuffle.each{|rule|
			what=rule.what
			from=rule.from

			if target.resource.get(what)<constructAtMost
				ok=true
				from.each{|f|	
					ok=false if target.resource.get(f[0])<f[1]
				}
				if ok
					from.each{|f|	target.resource.sub(f[0],f[1])}
					target.resource.add(what,1)
					break
				end
			end
		}

	end

	def whatToDo(man)
		man.hlJobMode[:done]||=0
		return :goResting if man.hlJobMode[:done]>=man.getAggression
		return :goHarvesting if whatToHarvestList.length>0 
		return :goProducing if whatToConstructList.length>0 # ATM this is always true
	end

	# what the stock of all resources should be
	def stockShouldBe
		5
	end

	def constructAtMost
		15
	end

	# for each resource get the target's stock; check if it's below stockShouldbe;
	# collect those with lower stock first
	def whatToHarvestList
		list=RESOURCES.map{|res|[res,getResourceNearing(res)]}.select{|p|p[1]<stockShouldBe}.sort{|a,b|a[1]<=>b[1]}.map{|r|r[0]}
	end

	# get resources of target and resources that men are already harvesting
	def getResourceNearing(resource)
		target.resource.get(resource)+hero.getMen.select{|man|man.hlJobMode[:what]==resource}.length
	end

	def whatToConstructList
		list=TARGETS.map{|res|[res,getTargetNearing(res)]}.select{|p|p[1]<constructAtMost}.sort{|a,b|a[1]<=>b[1]}.map{|r|r[0]}
	end

	def getTargetNearing(resource)
		target.resource.get(resource)+hero.getMen.select{|man|man.hlJobMode[:construct]==resource}.length
	end

	def getNextWithResource(res)
		getMap.getNext(target,res,1)
	end

	def error(text)
		raise ConstructException.new(text)
	end
end














class HLJob_Build<HLJob_CreationBase
	RESOURCES=["wood","stone"]

	include HLJob_HarvestModule
	include HLJob_RestingModule

	VALID_STATES=[
		:goHarvesting,:harvest,:comeBack,:deliverAll,
		:goBuilding,:build,:endBuild,
		:goResting,:sitDown,:rest,
		:goFlatten,:flatten,:endFlatten
	]

	def resources
		RESOURCES
	end

	def ready
		target.ready
	end

	private

	def goFlatten(man)
		possible=getWhatToFlatten
		position=possible[0]
		man.hlJobMode[:flattening]=position
		assert{not position.nil?}
		man.walkTo(AGVector2.new(position[0],position[1]))
		man.hlJobMode[:task]=:flatten
	end
	def flatten(man)
		man.hlJobMode[:task]=:endFlatten
		man.newRestJob(3,true)
		man.setMeshState("pick")
	end

	def endFlatten(man)
		p=man.hlJobMode[:flattening]
		assert{not p.nil?}
		v=getMap.get(p[0],p[1])*(1-p[2])+@flatheight*p[2]
		getMap.set(p[0],p[1],v)
		v=getMap.getTerrain(p[0],p[1],EARTH)*(1-p[2])+p[2]
		getMap.setTerrain(p[0],p[1],EARTH,v)
		getMap.endChange
		@flatpositions.delete(p)
		man.hlJobMode[:flattening]=nil
		man.hlJobMode[:task]=nil
	end
	def somethingToFlattenLeft
		getWhatToFlatten.length>0
	end

	def getWhatToFlatten
		if @flatpositions.nil?
			targetpos=target.getPos2D
	
			px=targetpos.x.to_i+1
			py=targetpos.y.to_i+1
			@flatpositions=[]
			(-3..3).each{|y|
				(-3..3).each{|x|
					v=1-Math::sqrt(x**2+y**2)/4.0
					v=[0,1,v*1.5].sort[1]
					@flatpositions.push([x+px,y+py,v])
				}
			}
			@flatheight=getMap.get(px,py)
			@flatpositions=@flatpositions.shuffle
		end
		
		@flatpositions-hero.getMen.collect{|man|man.hlJobMode[:flattening]}
	end

	# :section: Producing

	def goBuilding(man)
		puts "goBuilding #{man}"
		man.walkTo(target)
		man.hlJobMode[:task]=:build
	end

	def build(man)
		puts "build #{man}"
		man.hlJobMode[:task]=:endBuild
		man.newRestJob(3,true)
		man.setMeshState("pick")
	end

	def endBuild(man)
		man.incExperience(man.learnAmount)
		buildIncrease
	end

	def whatToDo(man)
		res=whatToDo2(man)
		print "whatToDo:"
		pp res
		res
	end

	def whatToDo2(man)
		man.hlJobMode[:done]||=0
		return :goResting if man.hlJobMode[:done]>=man.getAggression
		return :goFlatten if somethingToFlattenLeft
		return :goBuilding if enoughResourceLeft and not target.ready
		return :goHarvesting #if whatToHarvestList.length>0 # always
	end

	# what the stock of all resources should be
	def stockShouldBe
		5
	end

	# for each resource get the target's stock; check if it's below stockShouldbe;
	# collect those with lower stock first
	def whatToHarvestList
		list=RESOURCES.map{|res|[res,getResourceNearing(res),target.resource.get(res)]}.select{|p|p[1]<stockShouldBe or p[2]<2}.sort{|a,b|a[1]<=>b[1]}.map{|r|r[0]}
	end

	# get resources of target and resources that men are already harvesting
	def getResourceNearing(resource)
		target.resource.get(resource)+(hero.getMen-[hero]).select{|man|man.hlJobMode[:what]==resource}.length
	end

	def getNextWithResource(res)
		getMap.getNext(target,res,1)
	end

	def enoughResourceLeft
		alreadyBuilding=hero.getMen.select{|man|[:build,:goBuilding].member?(man.hlJobMode[:task])}.length+1
		puts "alreadyBuilding:#{alreadyBuilding}"
		#return true if alreadyBuilding==0
		neededResources=target.building.buildResources
		ok=true
		neededResources.each{|k,v|

			ok=false if target.resource.get(k)<v*alreadyBuilding
		}
		puts "ok: #{ok}"
		ok
	end

	def error(text)
		raise ConstructException.new(text)
	end


	def flattenLand
		targetpos=target.getPos2D


		px=targetpos.x.to_i+1
		py=targetpos.y.to_i+1
		if @flatpositions.nil?
			@flatpositions=[]
			(-3..3).each{|y|
				(-3..3).each{|x|
					v=1-Math::sqrt(x**2+y**2)/4.0
					v=[0,1,v*1.5].sort[1]
					@flatpositions.push([x+px,y+py,v])
				}
			}
			@flatheight=getMap.get(px,py)
			@flatpositions.shuffle
		end


		
		if @flatpositions.length>0
			(0..1).each{|i|
				break if @flatpositions.length==0
				p=@flatpositions.shift
				v=getMap.get(p[0],p[1])*(1-p[2])+@flatheight*p[2]
				getMap.set(p[0],p[1],v)
				v=getMap.getTerrain(p[0],p[1],EARTH)*(1-p[2])+p[2]
				getMap.setTerrain(p[0],p[1],EARTH,v)
			}
			getMap.endChange
			return true
		end
		false
	end

	def buildIncrease
		puts "buildIncrease"
		#return if flattenLand

		building=target.building
		neededResources=building.buildResources
	
		neededResources.each{|k,v|
			if target.resource.get(k)<v
				puts "NOT FOUDN: #{k}:#{v}"
				return # oooooh
			end
		}
		neededResources.each{|k,v|
			target.resource.sub(k,v)
		}

		puts "inc:",target.building.buildSteps
		puts "stepcount:",target.getStepCount
		target.incProgress(target.building.buildSteps)
		
		if target.ready
			puts "READY"
			# delete buildingsite and replace with building
			getMap.removeEntity(target)
			house=building.new(getMap)
			house.setPos(target.getPos2D)
			getMap.insertEntity(house)
			house.setPlayer(hero.getPlayer)
			house.setName(house.class.to_s.gsub("Ant",""))
			house.resource.takeAll(target.resource) # give remaining resources to house
		end
	end

end


