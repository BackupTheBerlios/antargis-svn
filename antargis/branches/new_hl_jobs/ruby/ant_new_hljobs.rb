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
	state :formatSit=>HLJob_FormatSit
	state :sitDown=>HLJob_SitDown
	state :justSitOnce=>HLJob_JustSitOnce
	state :spreadThings=>HLJob_SpreadThings

	startState :formatSit
	edge :formatSit, :sitDown
	edge :sitDown,:justSitOnce
	edge :justSitOnce,:spreadThings,:checkSpread
	edge :justSitOnce,:justSitOnce,:notCheckSpread
	edge :spreadThings,:formatSit,:lastSpread

	SPREAD_CHECK_TIME=10 # all 10 seconds spread things

	def initialize(hero,time)
		super(hero)
	end
	def image
		"data/gui/bed.png"
	end
	# fixme: discard this
	def makeMessage(boss)
		RestMessage.new(boss,@time)
	end

	def lastSpread
		@spreadTime=getTime
		true
	end

	def checkSpread
		curTime=getTime
		puts "SPREADTIME: #{@spreadTime}   getTime:#{curTime}"
		return true if @spreadTime.nil?
		puts "#{curTime-@spreadTime}>#{SPREAD_CHECK_TIME}"
		return curTime-@spreadTime>SPREAD_CHECK_TIME
	end

	def notCheckSpread
		not checkSpread
	end
		
end

AntHeroRestJobOld=AntHeroRestJob
AntHeroRestJob=AntNewHLRestJob



class AntNewHLMoveJob<AntNewHLJob
	state :moveComplete=>	HLJob_MoveComplete
	state :endState => HLJob_DummyState

	startState :moveComplete
	endState :endState

	edge :moveComplete,:endState

	attr_accessor :targetPos
	attr_accessor :formatDir

	def initialize(hero,prio,pos,dist,doFormat=true)
		@targetPos=pos.dim2
		super(hero)
		if not doFormat
			state.moveDirectly			
		end
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

AntHeroMoveJobOld=AntHeroMoveJob
AntHeroMoveJob=AntNewHLMoveJob





class AntNewHLTakeJob<AntNewHLJob
	state :fetchStart => HLJob_FetchStart
	state :getResource => HLJob_GetResource
	state :move =>HLJob_MoveComplete
	state :moveBack => HLJob_MoveComplete
	state :endState => HLJob_DummyState

	startState :move
	endState :endState

	edge :move,:fetchStart,:fetchStart
	edge :fetchStart,:getResource,:gettingResource
	edge :getResource, :moveBack
	edge :moveBack, :endState

	attr_accessor :targetPos, :near, :target

	def initialize(phero,target,what="food")

		@targetPos=target.getPos2D
		@target=target
		super(phero)
		@states[:move].near=4
		@states[:getResource].resources={"food"=>["food"],"weapon"=>["sword","bow","boat","shield"]}[what]
	end

	def image
		if @what=="food"
			"data/gui/take_apple.png"
		else
			"data/gui/take_weapon.png"
		end
	end

	def fetchStart
		@targetPos=hero.getPos2D
		true
	end
	def gettingResource
		true
	end
end

AntHeroTakeJobOld=AntHeroTakeJob
AntHeroTakeJob=AntNewHLTakeJob




class AntNewHLKillAnimal<AntNewHLTakeJob
	inheritMachine

	def gettingResource
		# kill animal
		playSound
		killAnimal
		super
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
AntHeroFightAnimalJobOld=AntHeroFightAnimalJob
AntHeroFightAnimalJob=AntNewHLKillAnimal



class AntNewHLFight<AntNewHLJob
	state :move=>HLJob_MoveComplete
	state :fight=>HLJob_Fight
	state :endState=>HLJob_DummyState

	edge :move,:fight
	edge :fight,:endState

	startState :move
	endState :endState

	attr_accessor :targetPos,:near,:target

	def initialize(hero,target,defend=false)
		@targetPos=target.getPos2D
		@target=target
		puts "DEFEND #{defend}"
		if defend
			trace
			super(hero,:fight)
			trace
		else
			trace
			super(hero)
			trace
		end
		assert{@state==:fight || defend==false}
		@states[:move].near=10

		puts "STATE #{state}"
	end

	def image
		"data/gui/sword.png"
	end

	def eventWon(opponent)
	end
	def eventLost(opponent)
		hero.setOwner(opponent)
	end


end

# rename and replace old hl-jobs




# AntHeroFightAnimalJobOld=AntHeroFightAnimalJob
# AntHeroFightAnimalJob=AntNewHLKillAnimal
# 
AntHeroFightJobOld=AntHeroFightJob
AntHeroFightJob=AntNewHLFight

if false

end