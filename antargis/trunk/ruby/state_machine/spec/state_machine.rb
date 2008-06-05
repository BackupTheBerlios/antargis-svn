# bowling_spec.rb
require 'ruby/state_machine/state_machine.rb'

describe StateMachineNode do
  before(:each) do
    @node = StateMachineNode.new(:id)
  end

  it "should have id :id" do
    @node.id.should == :id
  end

  it "should have a function dict, which is the state-machines dictionary" do
    @node.methods.member?("dict").should == true
  end

  it "accessing the dictionary when the node is not attached to a machine will raise an exception" do
    lambda {@node.dict}.should raise_error(AntargisException)
  end
end

describe StateMachineEdge do
  before(:each) do
    @e = StateMachineEdge.new(:a,:b,:name)
  end

  it "should have from node :a" do
    @e.from.should == :a
  end
  it "should have to node :b" do
    @e.to.should == :b
  end
  it "should have name :name" do
    @e.name.should == :name
  end
end

describe StateMachineDefinition, "with no node" do
  before(:each) do
    @d=StateMachineDefinition.new
  end
  it "creating a machine should raise an error" do
    lambda {@d.createMachine}.should raise_error(AntargisException) #("startnode not set")
  end
end

describe StateMachineDefinition, "with a single node" do
  before(:each) do
    @d = StateMachineDefinition.new
    @d.addNode(StateMachineNode,:startNode)
    @d.startNode=:startNode
  end
  it "should have node count 1" do
    @d.nodeCount.should == 1
  end

  it "should be finished when run" do
    m=@d.createMachine
    m.tick(1)
    m.finished.should == true
  end

end

# a mockstate that can be set to finished externally
class MockState<StateMachineNode
  attr_accessor :dummyVar

  def initialize(pid)
    super(pid)  
    @finished=true

  end
  def finished=(f)
    @finished=f
  end
  def ready
    @finished
  end

  def eventEnter
    logEvent("eventEnter")
  end
  def eventLeave
    logEvent("eventLeave")
  end
  def eventFrame(t)
    logEvent("eventFrame")
    t
  end

private
  def logEvent(name)
    dict["events"]||=[]
    dict["events"] << id.to_s+" "+name
  end
  
end

describe StateMachine, " with 2 mock-states " do
  before(:all) do 
    @d=StateMachineDefinition.new
    @n1=@d.startNode=@d.addNode(MockState,:start)
    @n2=@d.addNode(MockState,:end)
    @d.addEdge(@n1,@n2)
  end
  it "should not touch another machine with same definition in their states" do
    m1=@d.createMachine
    m2=@d.createMachine

    m1.state.dummyVar=20
    m2.state.dummyVar.should == nil
  end

  before(:each) do
    @m=@d.createMachine
  end
  
  it "should call eventEnter on startNode before calling eventFrame" do
    @m.tick(1)
    events=@m.dict["events"]
    events.length.should == 5
    events[0].should == "start eventEnter"
    events[1].should == "start eventFrame"
    events[2].should == "start eventLeave"
    events[3].should == "end eventEnter"
    events[4].should == "end eventFrame"
  end

  it "should not be finished without being started/tick called" do
    @m.finished.should == false
  end
  it "should be finished when tick was called" do
    @m.tick(1)
    @m.finished.should == true
  end
  it "- rest-time should be equal to initial time when calling tick" do
    time=1
    restTime=@m.tick(time)
    time.should restTime
  end
  it "should only process first node and stay there, when tick-time is 0" do
    @m.tick(0)
    @m.state.id.should == @n1
  end
  it "should end in @n2 and be finished, when tick-time is 1" do
    @m.tick(1)
    @m.state.id.should == @n2
    @m.finished.should == true
  end
  it "should return a node when calling state" do
    @m.state.should is_a?(StateMachineNode)
  end

  it "should have a dictionary" do
    @m.dict.should is_a?(Hash)
  end

  it "should be able to pass on information from one state to another" do  
    @m.dict["test"]=10
    @m.tick(1)
    @m.state.id.should == @n2
    @m.dict["test"].should == 10
  end

  it "should be able to pass on information from one state to another - saved and restored within state" do  
    @m.state.dict["test"]=10
    @m.tick(1)
    @m.state.id.should == @n2
    @m.state.dict["test"].should == 10
  end

  it "- states have link up to machine" do
    @m.state.machine.should == @m
  end  

  it "should return the state, when giving the correct name" do
    s1=@m.getState(:start)
    s1.should is_a?(StateMachineNode)
    s1.id.should == @n1
    s2=@m.getState(:end)
    s2.should is_a?(StateMachineNode)
    s2.id.should == @n2
  end
end

class DelayState<StateMachineNode
  def eventEnter
    @time=1
  end
  def eventFrame(t)
    d=[1,t].min
    @time-=d
    t-=d
    t
  end
  def ready
    @time<=0
  end
end

describe StateMachine, " with 2 mock-states and a decision-node" do
  before(:all) do
    @d=StateMachineDefinition.new
    @n1=@d.addNode(DelayState,:start)
    @n2=@d.addNode(DecisionNode, :decision) do |node|(node.dict[:continue] ? :continue : :return) end
    @n3=@d.addNode(DelayState,:end)
    @d.addEdge(@n1,@n2)
    @d.addEdge(@n2,@n3,:continue)
    @d.addEdge(@n2,@n1,:return)
    @d.startNode=@n1
  end
  before(:each) do
    @m=@d.createMachine
  end
  it "should return again and again when continue is false" do
    @m.state.id.should == @n1
    @m.tick(0.5)
    @m.state.id.should == @n1
    @m.tick(0.5)
    @m.state.id.should == @n2
    @m.tick(0.5)
    @m.state.id.should == @n1
  end
  it "should finish when continue is true" do
    @m.dict[:continue]=true
    @m.state.id.should == @n1
    @m.tick(0.5)
    @m.state.id.should == @n1
    @m.tick(0.5)
    @m.state.id.should == @n2
    @m.tick(0.5)
    @m.state.id.should == @n3
  end
end

