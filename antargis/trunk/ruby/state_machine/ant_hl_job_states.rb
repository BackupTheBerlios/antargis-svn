require 'ruby/state_machine/state_machine.rb'

class HLJob_New_BaseState<StateMachineNode
	def hero
		dict[:hero]
	end
	def allMen
		dict[:hero].getMen
	end
	def getTime
		getMap.getTime
	end
end

class HLJob_DirState<HLJob_New_BaseState
	def formatDir
		(targetPos-hero.getPos2D).normalized
	end
	def targetPos
		dict[:targetPos]
	end
end

class HLJob_FormatWalk_State<HLJob_DirState
	# needed: getTime

	# wait 5 seconds at max for formatting
	FORMAT_MAX_TIME=5

	def eventEnter
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
end