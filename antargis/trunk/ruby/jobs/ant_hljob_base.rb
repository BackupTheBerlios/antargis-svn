require 'jobs/ant_state_machine.rb'

class AntNewHLJob<BaseMachine
  include XMLSaver
  
  attr_reader :finished, :stopped
  # some magic here
  def initialize(hero,startState=nil)
    super(startState)
    @hero=hero
    assert{not @hero.nil?}

    @finished=false

    # FIXME: is this to be moved somewhere else ?
    #firstCall
  end
  
  def xmlName
    self.class.to_s
  end
  #def saveXML(n)
  #  saveXMLNode(n)
    #if hasState
    #  sNode=n.addChild("state")
    #  state.saveXMLNode(sNode)
    #end
  #end

  def getTime
    @hero.getMap.getTime
  end

  def allMen
    if @hero.is_a?(AntHero)
      @hero.getMen
    else
      @hero.getMen-[@hero]
    end
  end

  def hero
    @hero
  end

  def getMap
    hero.getMap
  end
  
  def getRand
    @hero.getRand
  end

  def stopJob
    @stopped=true
    stateCall("leave")
  end

  def check(man)
    assert{self.started}
    #raise 1
    if @state.nil?
      @finished=true
      return
    end
    #raise 1
    stateCall("assign",man)
    advance
    @finished=ready
  end

  def delete(man)
    puts "IGNORING AntNewHLJob::delete(#{man})"
    if man==@hero
      puts "STOPPING JOB - because hero died"
      @finished=true
      @state=nil
    end
  end


  def trace
    return
#     if @hero.nil?
#       puts "TRACE #{caller[0]} #{self}"
#     else
#       puts "TRACE #{caller[0]} #{self} #{hero} #{getTime}"
#     end
  end

  def kill
    stateCall("kill")
  end

  def loadXML(node)
    puts "FIXME !!!!!!!!!!!!!!!! AntNewHLJob::loadXML"
  end
end
