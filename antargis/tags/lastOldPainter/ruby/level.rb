#!/usr/bin/env ruby

class Level
	def eventTrigger(hero,t)
		return false
	end
	def eventOwnerChanged(ent)
	end

	# use this to end a level
	def wonLevel
		puts "WON!!!!!!!!!!!!!!!!!!"
		$app=GameResult.new("won")
	end
	def lostLevel
		$app=GameResult.new("lost")
	end
	def tellStory(title,text)
		$app.storyTalk(title,text)
	end
end