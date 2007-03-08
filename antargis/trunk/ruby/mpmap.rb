#
# AntMpMap adds some helper functions to AntRubyMap. These include 
# additional support for Mulitplayer-games (not yet completed !!)
#
#

require 'map.rb'

class AntMpMap<AntRubyMap
	def initialize(c,app,pScene,w,h,playerName="Rowen")
		super(app,pScene,w,h,playerName)
		@connection=c
	end

	def processMessage(message)
		case message
			when RestMessage
				getByUID(message.hero).newHLRestJob(message.time)
			when MoveMessage
				getByUID(message.hero).newHLMoveJob(0,message.pos,message.dist)
			when FightMessage
				if message.defend
					getByUID(message.hero).newHLFightJob(getByUID(message.target))
				else
					getByUID(message.hero).newHLFightJob(getByUID(message.target))
				end
			when NewPlayerMessage
				h=AntHero.new
				puts "C:#{message.pos.class}"
				h.setPos(message.pos)
				h.setName(message.name)
				insertEntity(h)
		end

	end

	def eventNewJobAssignedToBoss(boss)
		if @connection
			if boss.getPlayer==getPlayer
				puts "===================================================="
				puts boss.getJob
				puts "===================================================="
				m=boss.getJob.makeMessage(boss.uid)
				@connection.sendMessage(m)
				puts m
				puts "message sent?????"
			end
		end
	end

end