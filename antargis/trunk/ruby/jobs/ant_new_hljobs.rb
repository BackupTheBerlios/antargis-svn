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

require 'jobs/ant_hljob_states.rb'
require 'jobs/ant_hljob_base.rb'


class AntHeroRestJob<AntNewHLJob
	state :formatSit=>HLJob_FormatSit
	state :sitDown=>HLJob_SitDown
	state :justSitOnce=>HLJob_JustSitOnce
	state :spreadThings=>HLJob_SpreadThings
	state :endState=>HLJob_DummyState

	startState :formatSit
	endState :endState

	edge :formatSit, :sitDown
	edge :sitDown,:justSitOnce,:notRestFinished
	edge :sitDown,:endState,:restFinished

	edge :justSitOnce,:spreadThings,:checkSpread
	edge :justSitOnce,:justSitOnce,:notCheckSpread
	edge :spreadThings,:formatSit,:lastSpread

	SPREAD_CHECK_TIME=10 # all 10 seconds spread things

	def initialize(hero,time)
		super(hero)
		@restTime=time
		@startTime=getTime
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
		#puts "SPREADTIME: #{@spreadTime}   getTime:#{curTime}"
		return true if @spreadTime.nil?
		#puts "#{curTime-@spreadTime}>#{SPREAD_CHECK_TIME}"
		return curTime-@spreadTime>SPREAD_CHECK_TIME
	end

	def notCheckSpread
		not checkSpread
	end
		
	def restFinished
		finished=(getTime-@startTime)>@restTime
		#log "RESTJOB FINISHED: #{finished} ((#{getTime}-#{@startTime})>#{@restTime})"
		finished
	end
	def notRestFinished
		not restFinished
	end
end



class AntHeroMoveJob<AntNewHLJob
	state :moveComplete=>	HLJob_MoveComplete
	state :endState => HLJob_DummyState

	startState :moveComplete
	endState :endState

	edge :moveComplete,:endState

	attr_accessor :targetPos
	attr_accessor :formatDir
	attr_accessor :near

	def initialize(hero,prio,pos,dist,doFormat=true)
		@targetPos=pos.dim2
		@near=0
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



class AntHeroTakeJob<AntNewHLJob
	state :move =>HLJob_MoveComplete
	state :fetchStart => HLJob_FetchStart
	state :getResource => HLJob_GetResource
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
		@near=4
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
		@near=0
		true
	end
	def gettingResource
		true
	end
end



class AntHeroFightAnimalJob<AntHeroTakeJob
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


class AntHeroFightJob<AntNewHLJob
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
		@near=10
		#puts "DEFEND #{defend}"
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

		#puts "STATE #{state}"
	end

	def image
		"data/gui/sword.png"
	end

	def eventWon(opponent)
		log "eventWon hero:#{hero} opp:(#{opponent})"
	end
	def eventLost(opponent)
		log "eventLog hero:#{hero} opp:(#{opponent})"
		hero.setOwner(opponent)
	end


end


class AntHeroRecruitJob<AntNewHLJob
	state :moveComplete=>	HLJob_MoveComplete
	state :recruit=>HLJob_Recruit
	state :endState => HLJob_DummyState

	startState :moveComplete
	endState :endState

	edge :moveComplete,:recruit
	edge :recruit,:endState

	attr_accessor :targetPos
	attr_accessor :formatDir
	attr_accessor :target
	attr_accessor :near

	def initialize(hero,target)
		@targetPos=target.getPos2D
		@target=target
		@near=4
		super(hero)
		@states[:moveComplete].near=4
		
		if (hero.getPos2D-target.getPos2D).length<4
			state.moveDirectly			
		end
	end
	# FIXME: move this to a config-file !
	def image
		"data/gui/recruit.png"
	end
	# FIXME: discard this
	def makeMessage(boss)
		MoveMessage.new(boss,targetPos,@dist)
	end
end



class AntHeroConstructJob<AntNewHLJob
	state :moveComplete=>	HLJob_MoveComplete
	state :spreadThings=>HLJob_SpreadThings
	state :construct=>HLJob_Construct
	state :endState => HLJob_DummyState

	startState :moveComplete
	endState :endState

	edge :moveComplete,:spreadThings
	edge :spreadThings,:construct
	edge :construct,:endState

	attr_accessor :targetPos
	attr_accessor :formatDir
	attr_accessor :target

	def initialize(hero,target)
		@targetPos=target.getPos2D
		@target=target
		super(hero)
		@states[:moveComplete].near=4
		
		if (hero.getPos2D-target.getPos2D).length<4
			state.moveDirectly			
		end
	end
	# FIXME: move this to a config-file !
	def image
		"data/gui/construct.png"
	end
	# FIXME: discard this
	def makeMessage(boss)
		MoveMessage.new(boss,targetPos,@dist)
	end
end

class AntHeroBuildJob<AntNewHLJob
	state :moveComplete=>	HLJob_MoveComplete
	state :spreadThings=>HLJob_SpreadThings
	state :build=>HLJob_Build
	state :endState => HLJob_DummyState

	startState :moveComplete
	endState :endState

	edge :moveComplete,:spreadThings
	edge :spreadThings,:build
	edge :build,:endState

	attr_accessor :targetPos
	attr_accessor :formatDir
	attr_accessor :target
    attr_accessor :near
	
	def initialize(hero,target)
		@targetPos=target.getPos2D
		@target=target
        @near=4
		super(hero)
		
		if (hero.getPos2D-target.getPos2D).length<4
			state.moveDirectly			
		end
	end
	# FIXME: move this to a config-file !
	def image
		"data/gui/build.png"
	end
	# FIXME: discard this
	def makeMessage(boss)
		MoveMessage.new(boss,targetPos,@dist)
	end
end


# FIXME:
# 3) build houses
# saving/loading
