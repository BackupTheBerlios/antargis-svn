class AntNewHLJob
	attr_reader :finished
	# some magic here
	def initialize(hero)
		@hero=hero

		@finished=false

		firstCall
	end
	def getTime
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

	def check(man)
		return if @state.nil?
		call2(@state,"assign",man)
		while call(@state,"ready")
			call(@state,"leave")
			goToNextState
			break if @state.nil? or @finished
		end
		puts "FINISHED: #{@finished}"
	end

	# override this one
	def goToNextState
		@workflow.each{|p|
			if p[0]==@state
				@state=p[1]
				call(@state,"enter")
				return
			end
		}
		@finished=true
	end

	def switchToState(state)
		call(@state,"leave")
		@state=state
		call(@state,"enter")
	end

	def firstCall
		call(@state,"enter")
	end

	def call(state,event)
		m=methodName(state,event)
		print "CALL #{state} #{event} #{m}  #{getTime} -- "
		if self.class.method_defined?(m)
			v=send(m)
			puts v
			return v
		else
			puts "UNKNOWN"
			true
		end
	end
	def xmlName
		self.class.to_s
	end

	def call2(state,event,man)
		m=methodName(state,event)
		print "CALL2 #{state} #{event} #{m} #{man} #{getTime} -- "
		if self.class.method_defined?(m)
			v=send(m,man)
			puts v
			return v
		else
			puts "UNKNOWN"
			true
		end
	end

	def methodName(state,event)
		m=state.to_s+"_"+event
		m=m[0..0].downcase+m[1..-1]
	end

	def trace
		puts "TRACE #{caller[0]} #{getTime}"
	end
		
end
