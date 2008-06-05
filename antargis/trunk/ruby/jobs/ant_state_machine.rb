class BaseState
  attr_reader :started
  @@states={}
  @@edges={}
  @@endstates={}
  @@startstate={}
  def BaseState.state(p)
    @started=false
    s=@@states[self]
    s||={}
    p.each{|k,v|s[k]=v}
    @@states[self]=s
  end
  def BaseState.edge(v0,v1,f=nil)
    @@edges[self]||=[]
    @@edges[self]<<[v0,v1,f]
  end
  def BaseState.startState(s)
    #puts "setting startstate for #{self}:#{s}"
    @@startstate[self]=s
  end
  def BaseState.endState(s)
    @@endstates[self]||=[]
    @@endstates[self].push(s)
  end

  def BaseState.inheritMachine
    me=ancestors[0]
    anc=ancestors[1]
    @@states[me]=@@states[anc].dup
    @@edges[me]=@@edges[anc].dup
    @@endstates[me]=@@endstates[anc].dup
    @@startstate[me]=@@startstate[anc]
  end
  
  def initialize(startState=nil)
    @states={}
    @@states[self.class].each{|k,v|@states[k]=v.new;@states[k].machine=self}
    @state=startState
    @state||=@@startstate[self.class]
    @firstRun=false
  end
  
  def ready
    return true if @state.nil?
    if @@endstates[self.class]
      if @@endstates[self.class].length>0
        return true if @@endstates[self.class].member?(@state)
      end
    end
    return false
  end    

  def advance
    if @states.length==0
      puts "no states here"
      return
    end
    if hasState
      stepState
    end
  end

  def firstCall
    @started=true
    enterRecursive
  end

  def hasState
    if @state
      return (not @states[@state].nil?)
    end
    false
  end

  def state
    if @state
      if @states[@state]
        return @states[@state]
      end
    end
    raise "no state defined!"
  end
  
  def stateName
    @state
  end

  def stateCall(*s)
    if @state
      if @states[@state]
        #puts "(#{s})(#{s[0]})"
        if @states[@state].respond_to?(s[0])
          @states[@state].send(*s) #(s[1..-1]))
        end
      end
    end
  end

protected
  def enterRecursive(selftoo=true)
    #puts "#{self}:enterRecursive"

    if self.respond_to?("enter") and selftoo
      #puts "#{self}:enterRecursive - enter..."
      enter
      #puts "#{self}:enterRecursive - enter.ok"
    end

    #puts "#{self}:enterRecursive - hasState:#{hasState}"
    if hasState
      #puts "#{self}:enterRecursive - respond_to(enterRec):#{state.respond_to?("enterRecursive")}"
      #puts "#{self}:enterRecursive - state:#{state}"
      if state.respond_to?("enterRecursive")
        state.enterRecursive
      else
        #puts "#{self}:enterRecursive - statecall-enter..."
        stateCall("enter")
      end
    end
  end

private
  def stepState
    assert {hasState}
    stateCall("advance")
    if state.ready
      toNextState
    else
      true
    end
  end
  
  def toNextState
    #puts "#{self} toNextState:#{@state}:#{state}"
    nstate=nil
    @@edges[self.class].each{|e|
      if e[0]==@state
        ok=true
        if not e[2].nil?
          ok=self.send(e[2]) #.call
        end
        if ok
          nstate=e[1]
        end
      end
    }
    if nstate
      switch2(nstate)
      return true
    elsif @@endstates[self.class].member?(@state)
      switch2(nil)
    end
    false
  end
  
  def left
    @firstRun=false
  end
    
  def switch2(s)
    stateCall("leave")
    stateCall("left")
    @state=s
    #stateCall("enterRecursive")
    enterRecursive(false)
  end
end

class BaseMachine<BaseState
#   def initialize(start=nil)
#     super(start)
#     if start.nil?
#       @state=@@startstate[self.class]
#       puts "startstate: #{@@startstate[self.class]}"
#     else
#       @state=start
#     end
#   end
end

