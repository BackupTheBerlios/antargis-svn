class Level1<Level
	def initialize
		puts "Level inited"
	end
# 	def saveXML(n)
# 		super
# 		n.set("druid",@druid.to_s)
# 		n.set("druid2",@druid2.to_s)
# 		n.set("started",@started.to_s)
# 	end
# 	def loadXML(n)
# 		super
# 		@druid=n.get("druid")=="true"
# 		@druid2=n.get("druid2")=="true"
# 		@started=n.get("started")=="true"
# 	end

	def eventLevelStarted
		if not @started
			start=StoryFlow.new("beginning")
			start.push("Godrin","Slowly you awake.")
			start.push("Godrin","You remember that you have been attacked, but ...")
			start.push("Godrin","Anything else is gone. You've lost your memory.")
			tellStory(start)
			@started=true
		end
	end

	def eventTrigger(hero,t)
		case t.name
			when "nogo"
				if hero.getPlayer.class==AntHumanPlayer and not @druid
					s=StoryFlow.new("noFurther")
					s.push(hero.getName,"I will not go any further! There are strangers out there. They don't seem friendly.")
					tellStory(s)
					hero.newHLMoveJob(0,getMap.getTarget("goback").pos,0)
				end
			when "druid"
				if @druid	
					if not @druid2
						@druid2=true
						s=StoryFlow.new("druid2")
						s.push("Druid","Go your path hero.")
						
						tellStory(s)
					end
				else
					@druid=true
					s=StoryFlow.new("druid1")
					s.push("Druid","Greetings stranger! I am the druid.")
					s.push("Druid","These are dangerous times. Be watchful in these woods.")
					s.push("Godrin","I think I have lost my memory.")
					s.push("Druid","Yes, but I see times of remembrance in your future...")
					s.push("Druid","You should go east and meet the smith.")
					s.push("Godrin","But there is a party of strangers on the way.")
					s.push("Druid","They will be gone soon.")
					tellStory(s)
					getMap.getByName("Bantor").newHLMoveJob(0,getMap.getTarget("retreat").pos,0)
					getMap.getByName("Godrin").newHLRestJob(10)
				end
			when "storyFinished"
				if @story.name=="end"
					wonLevel
				end
		end
		return false # ignore
	end
	def eventOwnerChanged(ent)
		case ent.getName	
			when "Godrin"
				lostLevel
			when "Bantor"
				wonLevel
		end
	end
end

