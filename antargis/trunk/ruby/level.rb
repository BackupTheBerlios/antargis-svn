#!/usr/bin/env ruby

class Level
	def eventTrigger(hero,t)
		return false
	end
	def eventOwnerChanged(ent)
	end
	def eventHLJobFinished(hero,job)
	end
	def eventDismissed(hero)
	end

	# use this to end a level
	def wonLevel
		puts "WON!!!!!!!!!!!!!!!!!!"
		$app=GameResult.new("won")
	end
	def lostLevel
		$app=GameResult.new("lost")
	end
	def endLevel
		$app.tryQuit
	end
	def tellStory(flow)
		$app.storyTalk(flow)
		@story=flow
	end
end