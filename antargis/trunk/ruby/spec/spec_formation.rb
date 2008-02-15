require 'ruby/spec/level_testing.rb'

class MockResource
	def get(*s)
		0
	end
end

class MockMan
    def initialize(pos)
        @pos=pos
    end
	def getPos2D
		@pos
	end
	def resource
		MockResource.new
	end
end

class MockHero<MockMan
	def initialize(pos)
		super
		@men=[]
	end
	def add(man)
		@men << man
	end
	def getMen
		@men+[self]
	end
end


describe 'SitFormation' do
	it "should center on heroes position with hero only" do
		hero=MockHero.new(AGVector2.new(0,0))
		AntFormationRest.new(hero).getPosition(hero,hero.getPos2D).should == hero.getPos2D
	end
	
	(2..25).step(3){|count|
		it "should center on heroes with #{count} men in range of 1" do
			middle=AGVector2.new(0,0)
			some=AGVector2.new(100,100)
			hero=MockHero.new(middle)
			null=AGVector2.new(0,0)
			(1...count).each{|i|
				hero.add(MockMan.new(some))
			}
			formation=AntFormationRest.new(hero)
			poss=hero.getMen.collect{|man|
				pos=formation.getPosition(man,hero.getPos2D)
				puts pos
				pos
			}
			sum=poss.inject(null){|a,b|a+b}
			(sum/poss.length-middle).length.should < 1
		end
	}
end

describe 'WalkFormation' do
	it "should work correctly with only the hero"
	it "should work correctly with a single trooper"
	it "should work correctly with 5 troopers"
    it "should work correctly with 15 troopers"
    it "should work correctly with 35 troopers"
end