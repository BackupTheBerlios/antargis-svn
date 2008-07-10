class Effect<AGWidget
  def initialize(p,r,node)
    super(p,r)
    @registered=false
    @running=false
    @duration=node["duration"].to_f
    #run
  end
  def draw(p)
    if @registered==false
      registerEffect
    end
  end
  def run
    @running=true
    @time=0
  end
  def eventFrame(t)
    if @running
      @time+=t
      @running=false if @time>=@duration
      pp @running
      @time=@duration if @time>@duration 
      step(@time/@duration)
    end
  end
  def stop
    @running=false
  end
  def step(per)
    pp per
    exit
  end
  private
  def registerEffect
    if getApp
      getApp.sigFrame.connect(self,:eventFrame)
      @registered=true
    end
  end
end

class AppearEffect<Effect
  def initialize(p,r,node)
    super
    @name=node["name"]
    @target=node["table"]
    @row=node["row"].to_i
    @size=50
    
  end
  def step(amount)
    pp "AMOUNT:",amount
    table=getApp.getMainWidget.getChild(@target)
    table.modifyRow(@row,amount*@size)
  end
end

class HideEffect<Effect
  def initialize(p,r,node)
    super
    @name=node["name"]
    @target=node["table"]
    @row=node["row"].to_i
    #@size=50
    @size=nil
  end
  def step(amount)
    pp "AMOUNT HIDE:",amount
    @size||=table.getRow(@row)
    table.modifyRow(@row,(1-amount)*@size)
  end
  def table
    getApp.getMainWidget.getChild(@target)
  end
end
