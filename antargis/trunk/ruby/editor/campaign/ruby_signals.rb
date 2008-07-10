class RubySignal
  def initialize(name)
    @name=name
    @receivers=[]
  end
  def connect(object,method)
    @receivers<<[object,method]
  end
  def call(*s)
    @receivers.each{|p|
      object,method=p
      object.send(method,*s)
    }
  end  
end
def createSignal(x)
  signal=RubySignal.new(x)
  self.define_cmethod(x) {|*s|
    if s.length==0
      signal
    else
      signal.call(*s)
    end
  }
end
