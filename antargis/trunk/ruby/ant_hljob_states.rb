# Does the walk-formation
# 
#
module HLJob_FormatWalk
	# needed: getTime

	# wait 5 seconds at max for formatting
	FORMAT_MAX_TIME=5

	def formatWalk_enter
		trace
		hero.formation=AntFormationBlock.new(@hero,formatDir)
		heroPos=@hero.getPos2D
		allMen.each{|man|
			pos=@hero.getFormation(man,heroPos)
			man.walkTo(pos)
			man.hlJobMode[:formatting]=true
		}
		@formatStart=getTime
	end

	def formatWalk_assign(man)
		man.setDirection(180-(targetPos-hero.getPos2D).normalized.getAngle.angle*180.0/Math::PI)
		man.standStill
		man.hlJobMode.delete(:formatting)
	end

	def formatWalk_ready
		if getTime-@formatStart>FORMAT_MAX_TIME
			return true
		end
		allMen.each{|man|
			if man.hlJobMode[:formatting]
				return false
			end
		}
		return true
	end

private
	def formatDir
		(targetPos-hero.getPos2D).normalized
	end

end

module HLJob_FormatSit
	# needed: getTime

	# wait 5 seconds at max for formatting
	FORMAT_MAX_TIME=15

	def formatSit_enter
		trace
		hero.formation=AntFormationRest.new(@hero)
		heroPos=@hero.getPos2D
		allMen.each{|man|
			pos=@hero.getFormation(man,heroPos)
			man.walkTo(pos)
			man.hlJobMode[:formatting]=true
			#puts "formatSit_enter: #{man} to #{pos}  (heroPos:#{heroPos})"
		}
		@formatStart=getTime
	end

	def formatSit_assign(man)
		man.setDirection(180-(hero.getPos2D-man.getPos2D).normalized.getAngle.angle*180.0/Math::PI)
		man.standStill
		man.hlJobMode.delete(:formatting)
	end

	def formatSit_ready
		if getTime-@formatStart>FORMAT_MAX_TIME
			# FIXME:rest deserts
			return true
		end
		allMen.each{|man|
			#puts "formatSit_ready: #{man}:#{man.hlJobMode[:formatting]}"
			if man.hlJobMode[:formatting]
				return false
			end
		}
		return true
	end
end

module HLJob_MoveToNextWayPoint
	# needed: hero,allMen,targetPos, near
	def near
		0
	end

	def moveToNextWayPoint_enter
		hero.formation=AntFormationBlock.new(@hero,formatDir)
		allMen.each{|man|
			pos=@hero.getFormation(man,targetPos)
			man.newMoveJob(0,pos,near)
			man.hlJobMode[:walking]=true
		}
	end

	def moveToNextWayPoint_assign(man)
		man.standStill
		man.hlJobMode.delete(:walking)

		# FIXME: maybe let him desert if this is called too often ?
	end

	def moveToNextWayPoint_ready
		allMen.each{|man|
			if man.hlJobMode[:walking]
				return false
			end
		}
		return true
	end

end

module HLJob_SitDown
	def sitDown_enter
		allMen.each{|man|
			man.setDirection(180-(hero.getPos2D-man.getPos2D).normalized.getAngle.angle*180.0/Math::PI)
			man.sitDown
			man.hlJobMode[:sitting]=true
		}
	end

	def sitDown_assign(man)
		man.sitStill
		man.hlJobMode.delete(:sitting)
	end

	def sitDown_ready
		allMen.each{|man|
			if man.hlJobMode[:sitting]
				return false
			end
		}
		return true
	end
end

module HLJob_JustSit
	def justSit_enter
		allMen.each{|man|
			man.sitStill
		}
	end
	def justSit_assign(man)
		man.sitStill # and rest
	end

	def justSit_ready
		return false
	end
end

module HLJob_JustSitOnce
	def justSitOnce_enter
		allMen.each{|man|
			man.sitStill
			man.hlJobMode[:justSitting]=true
		}
	end
	def justSitOnce_assign(man)
		man.hlJobMode.delete(:justSitting)
	end
	def justSitOnce_ready
		allMen.each{|man|
			if man.hlJobMode[:justSitting]
				return false
			else
				return true
			end
		}
	end
end
	

module HLJob_FetchStart
	# needed a target-entity
	def fetchStart_enter
		trace
		fetchPoint=target.getPos2D
		puts "fetchPoint:#{fetchPoint}  heroPos:#{hero.getPos2D}"
		allMen.each{|man|
			man.walkTo(fetchPoint)
			man.hlJobMode[:fetching]=true
			puts "set to fetching: #{man}"
		}
		
	end

	def fetchStart_assign(man)
		trace
		man.standStill
		man.hlJobMode.delete(:fetching)
	end

	def fetchStart_ready
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

module HLJob_GetResource
	# needed: resources return array of resource-strings
	def getResource_enter
		allMen.each{|man|
			resources.each{|r|
				a=target.resource.get(r)
				if a>1
					man.resource.add(r,1)
					target.resource.sub(r,1)
				end
			}
		}
	end
	def getResource_ready
		return true
	end
end

module HLJob_KillAnimal
	def killAnimal_enter
		target.eventDie
		hero.resource.takeAll(target.resource)
	end
end

# spread things among team-members
module HLJob_SpreadThings
	RESOURCES_TO_SPREAD=["sword","shield","bow","boat"]

	def spreadThings_enter
		allMen.each{|man|
			man.walkTo(hero.getPos2D)
			man.hlJobMode[:gatherToSpread]=true
		}
		@alreadySpread=false
	end

	def spreadThings_assign(man)
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

	def spreadThings_ready
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
	end
	
end

class HLJob_FightData

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

	def remove(fightJob)
		@parties[fightJob.fightType].delete(fightJob)
		@oldparties[fightJob.fightType].delete(fightJob)
		reshuffle
	end

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
		assert{man.is_a?(AntHero) or man.is_a?(AntMan)}
		assert{man.getHero==fightJob.hero}
	
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
			return opponent
		end
	end

# 	def eventWon(whichType)
# 		trace
# 		
# 
# 		# FIXME: should be parted !!!
# 		owners=@oldparties[whichType].collect{|j|j.hero}
# 		i=0
# 		puts "whichType:#{whichType}"
# 		assert{owners.length>0}
# 		@oldparties[otherType(whichType)].each{|p|
# 			newOwner=owners[i]
# 			p.hero.setOwner(newOwner)
# 			i+=1
# 			i%=owners.length
# 		}
# 	end
# 	def eventLost(type)
# 		eventWon(otherType(type))
# 	end
	private

	def otherType(my)
		{:attacker=>:defender,:defender=>:attacker}[my]
	end

	def reshuffle
		# reinit and assign

		menGroup={}
		@parties.each{|type,jobs|
			puts "parties: #{type}:#{jobs}"
			menGroup[type]=jobs.collect{|job|job.undefeatedMen}.flatten.uniq
			return if @inited and menGroup[type].length==0 # a fightjob is leaving
		}
	
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

module HLJob_Fight
	# needed: hero,target, undefeatedMen

	attr_reader :fightType, :fightData, :won

	def fight_enter

		targetHadFightData=target.hlJobMode[:fightData].nil?
		
		puts "fightData: #{target.hlJobMode[:fightData]}"
		checkForFightData
		if targetHadFightData
			puts "NOT YET INITED #{self} hero:#{hero} target:#{target}"
			target.newHLDefendJob(hero)
		end
		#@fightData.init
		assignAllJobs
	end

	def fight_assign(man)
		if man.is_a?(AntHouse)
			man.newRestJob(20)
			return
		end

		if man.hlJobMode[:defeated]
			# FIXME:send away
			# FIXME: maybe flee ?
			if hero.is_a?(AntHouse)
				if man.hlJobMode[:homing]
					man.standStill
				else
					man.hlJobMode[:homing]
					man.walkTo(hero.getPos2D)
				end
			else
				# FIXME
			end
			man.hlJobMode[:defeated]=true
			man.hlJobMode.delete(:fighting)
		else
			opponent=@fightData.getNewOpponent(man,self)
			if opponent.nil?
				# won
				@won=true
			else
				man.hlJobMode[:fightTarget]=opponent
				man.newFightJob(0,man.hlJobMode[:fightTarget])
			end
		end
	end

	def fight_leave
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

	def fight_ready
		if won 
			eventWon(target)
			return true
		elsif lost
			eventLost(target)
			return true
		end
		return false
	end

	def lost
		(not @won) and undefeatedMen.length==0
	end

	def undefeatedMen
		allMen.select{|man|man.hlJobMode[:defeated].nil?}
	end

	private
	# return true if fightData already exists
	def checkForFightData
		if target.hlJobMode[:fightData]
			@fightData=target.hlJobMode[:fightData]
			@fightData.add(self)
			@fightType=@fightData.getFightType(self)
			return true
		else
			@fightData=HLJob_FightData.new(self)
			@fightType=:attacker
		end
		puts "assigned fightData!"
		hero.hlJobMode[:fightData]=@fightData # store so that it's avaiable above
	end
	def assignAllJobs
		# FIXME:assign a fight-job to every member (undefeatedMen)

		undefeatedMen.each{|man|
			assert{not man.hlJobMode[:fightTarget].nil?}
			man.newFightJob(0,man.hlJobMode[:fightTarget])
			# FIXME: check if morale is high enough
			man.hlJobMode.delete(:defeated)
			man.hlJobMode[:fighting]=true
		}
	end


end

