$:.push("ruby/entities")

require 'ruby/antargislib.rb'
require 'ruby/map.rb'
require 'ruby/entities/entities.rb'
require 'ruby/state_machine/ant_hl_job_states.rb'

# class MockEnt
#   def setPos(p)
#     @p=p
#   end
# end
# 
# class AntHero<MockEnt
#   def initialize
#     @men=[]
#   end
#   def addMan(m)
#     @men << m
#   end
# end
# class AntMan<MockEnt
#   def setBoss(b)
#     @boss=b
#     @boss.addMan(self)
#   end
# end

class JobTestingSuite
  attr_reader :hero, :entities, :men
  def initialize
    app=AGApplication.new
    scene=Scene.new(800,600)
    @map=AntRubyMap.new(app,scene,800,600)
    @hero=AntHero.new
    @hero.setPos(AGVector2.new(2,2))
    @men=[]
    vecs=[[2,1],[1,1]]
    vecs.each{|v|
      man=AntMan.new
      man.setPos(AGVector2.new(v[0],v[1]))
      man.setBoss(@hero)
      @men << man
    }
    @entities=@men+[@hero]
  end
end

describe HLJob_FormatWalk_State, "Formatting state" do
  # initialize a state-machine-def
  before(:all) do
    @def=StateMachineDefinition.new
    @def.startNode=@def.addNode(HLJob_FormatWalk_State,:format)
    @suite=JobTestingSuite.new    
  end
  before(:each) do
    @m=@def.createMachine
    @m.debug=true
    @m.dict[:hero]=@suite.hero
    @m.dict[:targetPos]=AGVector2.new(4,4)
  end

  it "should assign a job to each man" do
    @suite.entities.each{|e|e.hasJob.should == false }
    @m.tick(0.001)
    @suite.entities.each{|e|e.hasJob.should == true }
  end

  it "should format men to the given formation" do
    while not @m.finished do
      @m.tick(1)
      @suite.entities.each{|e|e.move(0.2)}
    end
    @suite.entities.each{|e|
      e.getPos2D.should == @suite.hero.getFormation(e,@suite.hero.getPos2D)
    }
  end
  
  it "should finish in a decent time."

  it "should assign even if old jobs are assigned."
end


