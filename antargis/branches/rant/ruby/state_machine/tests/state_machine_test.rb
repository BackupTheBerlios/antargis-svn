require 'ruby/state_machine/state_machine.rb'
require 'test/unit'

class MockState<StateMachineNode
	attr_accessor :dummyVar
	def initialize(pid)
		super(pid)	
		@finished=true
	end
	def finished=(f)
		@finished=f
	end
	def isFinished?
		@finished
	end
	
end


class SinglNodeTest < Test::Unit::TestCase
	def setup
		d=StateMachineDefinition.new
		
		n=StateMachineNode.new(:start)

		d.addNode(n)

		d.startNode=(n)

		@m=StateMachine.new(d)
	end

	def test_notFinishedWhenNotStarted
		assert((not @m.finished),"@m should not be finished when not yet started!")
	end

	def test_simplerun
		@m.tick(10)
		assert(@m.finished,"@m should be finished!")
	end
end

class TwoMachinesSeparate < Test::Unit::TestCase
	def setup
		d=StateMachineDefinition.new
		n=MockState.new(:p0)
		d << (n)
		d.startNode=n
		@m1=StateMachine.new(d)
		@m2=StateMachine.new(d)
	end

	def test_notSync
		@m1.state.dummyVar=10
		assert(@m2.state.dummyVar.nil?,"@m2.state.dummyVar shouldn't be influenced by @m1.state.dummyVar")
	end
end

