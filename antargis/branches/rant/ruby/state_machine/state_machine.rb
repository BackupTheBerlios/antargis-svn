require 'ruby/ant_tools.rb'

class StateMachineBase
	attr_accessor :dict
end

class StateMachineNode
	attr_reader :id
	attr_accessor :machine

	def initialize(pid)
		assert{pid.is_a?(Symbol)}

		@id=pid
	end

	def eventEnter
	end
	def eventLeave
	end
	def eventFrame(t)
		#raise "should not be used!"
		t-0.5
	end

	def assign(entity)
	end

	def ready
		true
	end

	def output
		nil
	end

	def dict
		assert{@machine.is_a?(StateMachine)}
		@machine.dict
	end
end

class StateMachineEdge
	attr_reader :from, :to, :name
	def initialize(from,to,name)
		assert{from.is_a?(Symbol)}
		assert{to.is_a?(Symbol)}
		assert{name.is_a?(Symbol)}
		@from=from
		@to=to
		@name=name
	end
end

class StateMachineDefinition
	attr_reader :startNode
	def initialize
		@nodes={}
		@edges={}
		@blocks={}
		@startNode=nil
	end

	def startNode=(name)
		assert("startNode is already a member node") {not @nodes[name].nil?}
		@startNode=name
	end

	def addNode(nodeClass,name,&block)
		assert {nodeClass.ancestors.member?(StateMachineNode)}
		assert {@nodes[name].nil?}
		@nodes[name]=nodeClass
		@blocks[name]=block
		name
	end
	def addEdge(node1,node2,name=nil)
		assert {hasNode(node1)}
		assert {hasNode(node2)}
		assert {name.is_a?(Symbol) || name.nil?}
		name||=:default
		edge=StateMachineEdge.new(node1,node2,name)
		@edges[edge.from]||=[]
		@edges[edge.from] << edge
	end

	def nodeCount
		@nodes.length
	end

	def ready?
		(not @startNode.nil?)
	end

	def getNextNode(nodeName,output)
		output||=:default
		return nil if @edges[nodeName].nil?
		#puts "ok"
		@edges[nodeName].each{|edge|
			#puts "#{edge.name} #{output}"
			if edge.name==output
				return edge.to
			end
		}
		nil
	end
	def hasNode(nodeName)
		(not @nodes[nodeName].nil?)
	end
	

	def getNodeClass(nodeName)
		assert("getNodeClass checks if has node '#{nodeName}'") {hasNode(nodeName)}
		@nodes[nodeName]
	end

	def createMachine
		StateMachine.new(self)
	end

	def getBlock(nodeName)
		@blocks[nodeName]
	end
end


class StateMachine
	MAX_LOOPS=20
	attr_reader :finished
	attr_accessor :dict
	attr_accessor :debug

	def initialize(definition)
		assert{definition.is_a?(StateMachineDefinition)}
		assert("startnode not set!") {definition.ready?}
		@definition=definition
		@nodes={}
		@dict={}
		@currentNode=getNode(@definition.startNode)
		@finished=false
		@started=false
	end

	def tick(timeFrame)
		restTime=timeFrame
		loops=0
		while restTime>0 and loops<MAX_LOOPS
			if not @started
				@started=true
				puts "#{@currentNode.id}.eventEnter" if @debug
				@currentNode.eventEnter
			end
			restTime=@currentNode.eventFrame(restTime)
			assert{restTime.is_a?(Numeric)}
			if @currentNode.ready
				output=@currentNode.output
				nextNodeName=@definition.getNextNode(@currentNode.id,output)
				#sputs "FROM #{@currentNode.id} TO #{nextNodeName}"
				if nextNodeName.nil?
					setFinished
					return eventFinished
				end
				nextNode=getNode(nextNodeName)
				@currentNode.eventLeave
				@currentNode=nextNode
				nextNode.eventEnter
			end
			loops+=1
		end
		raise "Too many loops in StateMachine.tick(.) restFrame:#{restTime}" if loops>=MAX_LOOPS
	end

	def eventFinished
	end

	def state
		@currentNode
	end

	def getState(state)
		getNode(state)
	end

	private
	def setFinished
		@finished=true
	end

	def getNode(nodeName)
		block=@definition.getBlock(nodeName)
		if block
			@nodes[nodeName]||=@definition.getNodeClass(nodeName).new(nodeName,block)
		else
			@nodes[nodeName]||=@definition.getNodeClass(nodeName).new(nodeName)
		end
		@nodes[nodeName].machine=self
		@nodes[nodeName]
	end
end

class DecisionNode < StateMachineNode
	def initialize(id,block)
		super(id)
		@block=block
	end
	def output
		@block.call(self)
	end	
end

