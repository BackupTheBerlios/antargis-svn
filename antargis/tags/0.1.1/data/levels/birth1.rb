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
						s.push("Druid","Go your path hero. I have important things to do.")
						
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
			when "smith"
				if @smith==nil
					s=StoryFlow.new("smith0")
					s.push("Smith","Come here my friend!")
					tellStory(s)
					@smith=1
				end
			when "smith2"
				if @smith==1
					s=StoryFlow.new("smith1")
					s.push("Smith","Welcome to our small town! What is your name, boy?")
					s.push("Godrin","My is name is .. well, I don't remember it.")
					s.push("Smith","Then he is surely the man, the druid mentioned.")
					s.push("Godrin","What do you mean?")
					s.push("Smith","Nothing. You look like a brave young man to me.")
					s.push("Smith","If you promise to act wisely and defeat those men in the north, you can have some of my men.")
					s.push("Godrin","Yes, these attacked my party - as much as this I remember.")
					s.push("Smith","Well, then recruit some of my men.")
					tellStory(s)
					# assign houses to Player Godrin
					["Farmstead","Farm","Workshop"].each{|n|getMap.getByName(n).setPlayer(getMap.getPlayer)}
					@smith=2
				end
			when "storyFinished"
				case @story.name
					when "end"
						wonLevel
					when "smith0"
						getMap.getByName("Godrin").newHLMoveJob(0,getMap.getTarget("near_smith").pos,0)
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

