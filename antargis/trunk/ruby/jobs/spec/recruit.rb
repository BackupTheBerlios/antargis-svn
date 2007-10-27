require 'ruby/antargislib.rb'
require 'map.rb'
require 'entities.rb'
#require 'ruby/state_machine/ant_hl_job_states.rb'

module Testing
	@@testing=false
	def Testing.initTesting
		return if @@testing
		@@testing=true

		@@app=AGApplication.new
		@@scene=Scene.new(800,600)
	end

	def Testing.getTestMapFor(test)
		initTesting
		case test
			when :recruit
				#map=AntRubyMap.new(@@app,@@scene,800,600)
				map=AntRubyMap.new(nil,nil,800,600)
				map.loadMap("data/levels/tutorial/tutorial0.antlvl")
				map
		end
	end
end

describe AntHeroRecruitJob,"Test-map for recruiting" do
	before(:all) do
		@map=Testing.getTestMapFor(:recruit)
	end
	it "should include a hero called 'Rowen'" do
		@map.getByName("Rowen").should is_a?(AntHero)
	end
end

describe AntHeroRecruitJob,"A hero recruit job" do
	before(:each) do
		@map=Testing.getTestMapFor(:recruit)

		@hero=@map.getByName("Rowen")
	end

	it "map should include Rowen" do
		@hero.should_not be_nil
	end


# 	it "should at any time determine how strong the aggression of the hero is and thus should not start new ll-jobs to recruit people if there are already enough"
# 	it "should take care when the aggression level is raised"
# 	it "should assign a low-level job to the men - according to the amount of needed people" 
# 	it "should return each man after recruiting of being recruited to the starting point"
# 	it "should use sit-formation for the starting-point"
# 	it "should let people sit, when they don't have anything to do"
# 
# 	it "aggression of 3 implies all people"		
# 	
# 	it "aggression of 2 implies 2/3 of all people"
# 	it "aggression of 1 implies 1/3 of all people"
end