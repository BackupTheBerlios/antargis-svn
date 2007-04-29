require 'ant_state_machine.rb'

class AntNewHLJob<BaseMachine
	attr_reader :finished, :stopped
	# some magic here
	def initialize(hero,startState=nil)
		super(startState)
		@hero=hero
		assert{not @hero.nil?}

		@finished=false

		firstCall
	end

	def getTime
		puts "getTime"
		@hero.getMap.getTime
	end

	def allMen
		if @hero.is_a?(AntHero)
			@hero.getMen
		else
			@hero.getMen-[@hero]
		end
	end

	def hero
		@hero
	end

	def stopJob
		@stopped=true
		stateCall("leave")
	end

	def check(man)
		#raise 1
		if @state.nil?
			@finished=true
			return
		end
		#raise 1
		stateCall("assign",man)
		advance
		@finished=ready
	end

	def delete(man)
		puts "IGNORING AntNewHLJob::delete(#{man})"
		if man==@hero
			puts "STOPPING JOB - because hero died"
			@finished=true
			@state=nil
		end
	end


	def trace
		if @hero.nil?
			puts "TRACE #{caller[0]} #{self}"
		else
			puts "TRACE #{caller[0]} #{self} #{hero} #{getTime}"
		end
	end

	def kill
		stateCall("kill")
	end		
end
