#
# This is a new implementation of the high-level jobs.
# They're now state-based. Each job consists of several
# states. Each of these states has (or can have) an "enter",
# "assign","ready" and a "leave"-function.
# "enter" is called each time the current job switches into
# this state. "leave" is called when the state is left
# "ready" will be called to check, if the task of this state is 
# finished and the job can be transfered into a new state.
# "assign" is called whenever a man has no low-level-job anymore
# and wants to get a new task. You should always assign low-level-jobs
# to waiting men, because otherwise this function gets called for
# each waiting man in each frame, which can have impact on the performance.
#
# 

def newHLJobs
	return true
end

# 
# FIXME: * add path-finding to moving
#        * do FIXME s ;-)

module HLJob
	def hero
		@hero
	end
	def allMen
		@hero.getMen
	end
end

require 'ant_hljob_states.rb'
require 'ant_hljob_base.rb'


class AntNewHLRestJob<AntNewHLJob
	include HLJob_FormatSit
	include HLJob_SitDown
	include HLJob_JustSitOnce
	include HLJob_SpreadThings

	SPREAD_CHECK_TIME=10 # all 10 seconds spread things

	def initialize(hero,time)
		@state=:FormatSit
		@workflow=[[:FormatSit,:SitDown],[:SitDown,:CheckSpread],[:SpreadThings,:FormatSit],[:JustSitOnce,:CheckSpread]] #JustSit],[:JustSit]
		super(hero)
	end
	def image
		"data/gui/bed.png"
	end
	# fixme: discard this
	def makeMessage(boss)
		RestMessage.new(boss,@time)
	end

	def sitDown_leave
		if @spreadOnce.nil?
			@spreadTime=getTime-SPREAD_CHECK_TIME
			@spreadOnce=true
		else
			@spreadTime=getTime
		end
	end
	

	def checkSpread_enter
		curTime=getTime
		puts "#{curTime-@spreadTime}>#{SPREAD_CHECK_TIME}"
		if curTime-@spreadTime>SPREAD_CHECK_TIME
			switchToState(:SpreadThings)
		else
			switchToState(:JustSitOnce)
		end
	end
end

class AntNewHLMoveJob<AntNewHLJob
	include HLJob_FormatWalk
	include HLJob_MoveToNextWayPoint
	include HLJob_FormatSit
	include HLJob_SitDown
	# FIXME: time is irrelevant here!
	#def initialize(hero,time)

	attr_reader :targetPos

	def initialize(hero,prio,pos,dist,doFormat=true)
		@targetPos=pos.dim2
		if doFormat
			@state=:FormatWalk
		else
			@state=:MoveToNextWayPoint
		end
		@workflow=[
			[:FormatWalk,:MoveToNextWayPoint],
# 			[:MoveToNextWayPoint,:FormatSit],
# 			[:FormatSit,:SitDown]
		]
		super(hero)
	end
	# FIXME: move this to a config-file !
	def image
		"data/gui/move.png"
	end
	# FIXME: discard this
	def makeMessage(boss)
		MoveMessage.new(boss,targetPos,@dist)
	end

end


class AntNewHLTakeJob<AntNewHLJob
	include HLJob_FormatWalk
	include HLJob_FetchStart
	include HLJob_GetResource
	include HLJob_MoveToNextWayPoint

	attr_reader :resources, :targetPos, :near, :target

	def initialize(phero,target,what="food")
		@resources={"food"=>["food"],"weapon"=>["sword","bow","boat","shield"]}[what]

		@state=:FormatWalk
		@workflow=[
			[:FormatWalk,:MoveToNextWayPoint],
			[:MoveToNextWayPoint,:MyStore],
			[:MyStore,:FetchStart],
			[:FetchStart,:GetResource],
			[:GetResource,:MoveToNextWayPoint]
		]
		@targetPos=target.getPos2D
		@target=target
		@near=4
		super(phero)
	end

	def image
		if @what=="food"
			"data/gui/take_apple.png"
		else
			"data/gui/take_weapon.png"
		end
	end


	def myStore_enter
		trace
		if @taken
			@finished=true
			return 
		end
		@targetPos=hero.getPos2D
		puts "#{@targetPos} #{hero.getPos2D}"
		@near=0
		@taken=true
	end
end

class AntNewHLKillAnimal<AntNewHLTakeJob
	def fetchStart_leave
		# kill animal
		playSound
		killAnimal
	end

	
	def myStore_enter
		if @taken
			@finished=true
			return 
		end
		# no need to return to separate position
		@taken=true
	end

	private
	def playSound
		# FIXME: play eat sound
	end
	def killAnimal
		@target.eventDie
		hero.resource.takeAll(target.resource)
	end
end

class AntNewHLFight<AntNewHLJob
	include HLJob_FormatWalk
	include HLJob_MoveToNextWayPoint
	include HLJob_Fight

	attr_reader :targetPos,:near,:target

	def initialize(hero,target,defend=false)
		@target=target
		if defend
			@state=:Fight
			@workflow=[]
			#trace
		else
			@state=:FormatWalk
			@targetPos=target.getPos2D
			@near=10
			@workflow=[
				[:FormatWalk,:MoveToNextWayPoint],
				[:MoveToNextWayPoint,:Fight]
			]
		end
		super(hero)
	end

	def image
		"data/gui/sword.png"
	end

	def eventWon(opponent)
	end
	def eventLost(opponent)
		hero.setOwner(opponent) #.getPlayer)
	end


end

# rename and replace old hl-jobs

AntHeroRestJobOld=AntHeroRestJob
AntHeroRestJob=AntNewHLRestJob

AntHeroMoveJobOld=AntHeroMoveJob
AntHeroMoveJob=AntNewHLMoveJob

AntHeroTakeJobOld=AntHeroTakeJob
AntHeroTakeJob=AntNewHLTakeJob

AntHeroFightAnimalJobOld=AntHeroFightAnimalJob
AntHeroFightAnimalJob=AntNewHLKillAnimal

AntHeroFightJobOld=AntHeroFightJob
AntHeroFightJob=AntNewHLFight