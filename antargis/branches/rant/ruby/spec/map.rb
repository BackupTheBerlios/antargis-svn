require 'ruby/antargislib.rb'
require 'map.rb'

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
				map=AntRubyMap.new(@@app,@@scene,800,600)
				map.loadMap("data/levels/tutorial/tutorial0.antlvl")
				map
		end
	end
end

describe AntRubyMap,"A BoA-Ruby-map" do
	it "should run without UI at all" do
		map=AntRubyMap.new(nil,nil,64,64)
		map.move(1)
	end

	it "should be possible to create two maps concurrently (low prio)"
end