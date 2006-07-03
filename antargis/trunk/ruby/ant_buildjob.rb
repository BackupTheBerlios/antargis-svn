class AntHeroBuildJob<AntHeroMoveJob
	attr_reader :finished
	def initialize(hero,targetpos,building)
		super(hero,0,target.getPos2D,4)
		@targetpos=targetpos
		@building=building
	end
	def image
		"data/gui/build.png"
	end
	def check(man)
		if moveFinished
			@hero.formation=AntFormationRest.new(@hero)
			if man.is_a?(AntBoss)
				checkEat(man)
				if not @constructStarted
					@men.each{|m|m.delJob}
					@constructStarted=true
				end
				man.newRestJob(20) # do nothing at all
				return
			end
			@constructStarted=true
			wantmen=(@men.length-1)*@hero.getAggression/3.0
			case man.getMode
				when "fetch" # go to resource
					res=getNeededResources
					res,nearest=getNextWithResource(res)
					@restype[man]=[res,nearest]
					if nearest.nil?
						puts "resource not found! #{res}"
						man.newRestJob(5)
					else
						man.newMoveJob(0,nearest.getPos2D,0.5) # near but not actually there
						man.setMode("harvest")
					end
				when "harvest" # harvest resource
					man.newRestJob(2)
					man.digResource(@restype[man][0])
					man.setMode("collect")
				when "collect" # bring back
					# FIXME: add sub resource from nearest
					p=@restype[man]
					amount=[p[1].resource.get(p[0]),man.canCarry].min
					p[1].resource.sub(p[0],amount)
					man.resource.add(p[0],amount)
					man.newMoveJob(0,@target.getPos2D,0)
					man.collectResource(@restype[man][0])
					man.setMode("brought")
				when "brought"
					man.setMode("after_brought")
					man.newRestJob(1)
					# take all natural resources - but not food and weapons - otherwise men starve!
					myResources.each{|r|
						@target.resource.take(man.resource,r)
					}
				when "after_brought"
					fpos=@hero.getFormation(man,@hero.getPos2D)
					man.newMoveJob(0,fpos,0)
					man.setMode("construct_torest")
				when "to_construct"  # do some constructing
					@target.incSmoke
					man.newRestJob(5 - @hero.getAggression*0.5) # work for 3.5-5 seconds (depending on aggression)
					man.setMode("constructing")
					man.playSound("construct")
				when "constructing"
					# was constructing
					readyConstructed
					man.incExperience(man.learnAmount)
					@target.decSmoke
					man.setMode("construct_torest")
					fpos=@hero.getFormation(man,@hero.getPos2D)
					man.newMoveJob(0,fpos,0)
				when "construct_torest"
					man.setMode("rest")
					man.newRestJob(3/@hero.getAggression) # shorter pauses when aggression is higher
					if man.getAggression<3
						checkEat(man)
					end
					@usedmen-=1
				else
					if wantmen>@usedmen
						@usedmen+=1
						man.newMoveJob(0,@target.getPos2D,0)
						if enoughResources
							man.setMode("to_construct")
						else
							man.setMode("fetch")
						end
					end
			end
		else
			super(man)
		end
	end
private
	def myResources
		["wood","stone","coal","ore"]
	end
	def enoughResources
		# FIXME support more resources
		m=myResources.min{|a,b|@target.resource.get(a)<=>@target.resource.get(b)}
		value=@target.resource.get(m)
		return value>5
		#(@target.resource.get("stone")>5 and @target.resource.get("wood")>5)
	end
	def getNeededResources
		m=myResources.sort{|a,b|@target.resource.get(a)<=>@target.resource.get(b)}
		puts "NEEDED: #{m}"
		return m
	end
	def getNextWithResource(res)
		#goods={"wood"=>"tree","stone"=>"stone","food"=>"tree","coal"=>"mine","ore"=>"mine"}
		#enttype=goods[res]
		ret=nil
		res.each{|r|
			ret=getMap.getNext(@target,r,1)
			if not ret.nil?
				ret=[r,ret]
				break
			end
		}

		ret
	end
	def checkEat(man)
		puts "CHECKEAT"
		if man.getFood<0.5
			if man.resource.get("food")>0
				man.incFood(1)
				man.resource.sub("food",1)
			elsif heroHasFood
				man.incFood(1)
				@hero.resource.sub("food",1)
			end
		end
	end
	def heroHasFood
		@hero.resource.get("food")>0
	end
	def readyConstructed
		# produce any
		@productionRules.shuffle.each{|rule|
			ok=true
			
			rule.from.each{|w,n|
				puts "#{w},#{n}"
				if @target.resource.get(w)<n
					ok=false
				end
			}
			if ok
				# we found a useful production
				rule.from.each{|w,n|@target.resource.sub(w,n)}
				@target.resource.add(rule.what,1)
				return #out
			end
		}
		# something went wrong - we don't have enough resources - whatever
	end
end
