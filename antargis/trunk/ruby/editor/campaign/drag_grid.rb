require 'ruby/antargislib.rb'
require 'pp'

# TODO:
# * put all draggable-objects within context of DragEnvironemt


module Arrows
  def createArrowPolies(from,to,width)
    a=createArrowLine(from,to,width)
    [a[0..1]+a[5..6],a[2..4]]
  end
  
  def createArrowLine(from,to,width)
    dir=(to-from).normalized
    y=dir
    x=y.normal
    v=[
      from+x*width,
      to+x*width-y*2*width,
      to+x*2*width-y*2*width,
      to,
      
      to-x*2*width-y*2*width,
      to-x*width-y*2*width,
      from-x*width
      ]
  end
  def getArrowTriangles(from,to,width)
    puts "gettin tris"
    ps=createArrowPolies(from,to,width)
    ts=[ps[0][0..2],ps[0][1..3],ps[1]]
    r=ts.map{|t|AGTriangle2.new(t[0],t[1],t[2])}
    puts "gettin tris!"
    r
  end
end


module Antargis
  class AGPainter
    include Arrows
    
    def drawRoundRect(rect,radius,color)
      drawPoly(createRoundRect(rect,radius),color)
    end
    def fillRoundRect(rect,radius,color)
      fillPoly(createRoundRect(rect,radius),color)
    end
    
    def drawArrow(from,to,width,color,borderColor)
      createArrowPolies(from,to,width).each{|p|
        fillPoly(p,color)
      }
      a=createArrowLine(from,to,width)
      drawPoly(a,borderColor)
    end
    

    private
    
    def createRoundRect(rect,radius)
      x=AGVector2.new(radius,0)
      y=AGVector2.new(0,radius)
      poly=
        createArc(rect.getV0+x+y,radius,0,90)+
        createArc(rect.getV10-x+y,radius,90,180)+
        createArc(rect.getV1-x-y,radius,180,270)+
        createArc(rect.getV01+x-y,radius,270,360)
    end
    
    def createArc(center,radius,a0,a1)
      tiling=8
      tiling*=2 if radius>5
      tiling*=2 if radius>10
      tiling*=2 if radius>20
      a0=270-a0
      a1=270-a1
      diff=(a1-a0).abs
      tiles=(diff/360.0)*tiling
      (0..tiles).map{|t|
        a=a0+(a1-a0)*t/tiles
        a*=Math::PI/180.0
        center+AGVector2.new(Math::sin(a),Math::cos(a))*radius
      }
    end
  end
  
  class AGWidget
    def getRoot
      return self if getParent.nil?
      getParent.getRoot
    end
    def getAncestor(type)
      return self if self.is_a?(type)
      return getParent.getAncestor(type) if getParent
      nil
    end
    def getAllDescendants
      getChildren.map{|child|[child]+child.getAllDescendants}.flatten
    end
    def moveToContext(to)
      o=getScreenRect
      p=getParent
      p.removeChild(self)
      to.addChild(self)
      setRect(o+(-to.getScreenRect.getV0))
    end
  end
end

class Effect<AGWidget
  def initialize(p,r,node)
    super(p,r)
    @registered=false
    @running=false
    @duration=node["duration"].to_f
    run
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
      @time=@duration if @time>@duration 
      step(@time/@duration)
    end
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
    table=getApp.getMainWidget.getChild(@target)
    table.modifyRow(@row,amount*@size)
  end
end


class AGHoverWidget<AGWidget
  attr_reader :hovered
  attr_accessor :hoverBorder
  def initialize(*s)
    super
    @hovered=false
    @hoverBorder=5
  end
  def eventMouseMotion(e)
    r=super
    @hovered=getScreenRect.shrink(@hoverBorder).contains(e.getMousePosition)
    r
  end
end

class GenericLayoutCreator<AGLayoutCreator
  def initialize(c)
    super()
    @c=c
  end
  def create(p,r,node)
    options=node.getAttributes
    pp @c
    setResult @c.new(p,r,options)
  end
end

def standardLayoutCreator(pClass)
  name=pClass.to_s
  name=name[0..0].downcase+name[1..-1]
  creator=GenericLayoutCreator.new(pClass)
  getLayoutFactory.addCreator(name,creator)
end

class DragEnvironment<AGWidget
  def initialize(p,r,options)
    super(p,r)
    @grid=nil
  end
  def getDragTargets
    getAllDescendants.select{|w|w.is_a?(DragTarget)}
  end
  def draw(p)
    super
    if @grid.nil?
      @grid=getAllDescendants.select{|w|w.is_a?(DragGrid)}
      getAllDescendants.select{|w|w.is_a?(DragBox)}.each{|w|w.grid=@grid}
    end
  end
end

class DragTarget<AGHoverWidget
  def initialize(p,r,options,grid=nil)
    super(p,r)
    options||={}
    @grid=grid
  end
  def free
    getChildren.length==0
  end
end

class DragTrash<DragTarget
  def draw(p)
    p.fillRoundRect(getRect.origin,10,AGColor.new(0x33,0x33,0x33))
  end
end

module DragObject
  
end

class DragBox<AGHoverWidget
  BORDER_WIDTH=15
  
  include DragObject
  
  attr_accessor :text, :grid
  attr_accessor :selected
  
  def initialize(p,r,text)
    super(p,r)
    @dragging=true
    @text=AGStringUtf8.new(text)
    @textPos=AGVector2.new(10,10)
    @lastCell=nil
    @color=AGColor.new(0,0xAA,0) 
    @selected=false
    self.hoverBorder=BORDER_WIDTH
    @@font||=AGFont.new("FreeSans.ttf",14)
  end
  def draw(p)
    r=getRect.origin.shrink(2)
    r2=r.shrink(BORDER_WIDTH)
    radius=8
    c1=@color
    c2=@color
    if @hovered or @dragging
      c2*=1.2
    end
    
    p.fillRoundRect(r.grow(2),radius,c1*0.3) if @selected
    p.fillRoundRect(r,radius,c1)
    p.fillRoundRect(r2,radius,c2)
    p.renderText(@text,@textPos,@@font)
  end
  def eventDragBy(e,v)
    super
    if @dragging
      setRect(getRect+v)
      return true
    end
    false
  end
  def getDragEnvironment
    getAncestor(DragEnvironment)
  end
  def getDragGrid
    getDragEnvironment.getAllDescendants.select{|c|c.is_a?(DragGrid)}[0]
  end
  
  def startDragging
    moveToContext(getDragEnvironment)
  end
  
  def eventMouseButtonDown(e)
    r=super
    sRect=getScreenRect
    if sRect.contains(e.getMousePosition)
      
      getDragGrid.select(self) if getDragGrid
	    if sRect.shrink(BORDER_WIDTH).contains(e.getMousePosition)
	      @dragging=true
	      startDragging
        return true
	    elsif @lastCell
	      startLine(e)
        return true
	    end
    end
    r
  end
  
  def startLine(e)
    puts "STARTLINE"
    env=getAncestor(DragGrid)
    env.addChild(line=DragLine.new(env,env.getRect,self))
    line.eventMouseButtonDown(e)
  end
  
  def eventMouseButtonUp(e)
    return super unless @dragging
    #moveToContext()
    cells=getDragEnvironment.getDragTargets
    scells=cells.select{|c|c.getScreenRect.shrink(10).contains(getMiddle)}
    cell=scells[0]
    
    if cell
      if cell.free
        assignCell(cell)
      elsif @lastCell
        assignCell(@lastCell)
	    else
	      hide
	    end
    elsif @lastCell
      assignCell(@lastCell)
    else
	    hide
    end
    
    if cell.is_a?(DragTrash)
	    pp cell
	    del 
    end
    @dragging=false
    true
  end
  private
  def del
    hide
    getParent.removeChild(self)
  end
  def assignCell(cell)
    @lastCell=cell
    moveToContext(@lastCell)
    centerObject
  end
  def centerObject
    setRect(getRect+(getParent.getRect.origin.getMiddle-getRect.getMiddle))
  end
  def release
  end
  def getMiddle
    getScreenRect.getMiddle
  end
end

class DragBoxStory<DragBox
  def initialize(p,r,text)
    super
    @color=AGColor.new(0xFF,0xAA,0)
  end
end

class DragLine<AGWidget
  include Arrows
  def initialize(p,r,startObject)
    super(p,r)
    @startObject=startObject
    @endObject=nil
    @pos=nil
    @@font||=AGFont.new("FreeSans.ttf",14)
    @@font.setColor(AGColor.new(0,0,0))
    @hovered=false
  end
    
  def getPos(obj)
    obj.getScreenRect.getMiddle-getScreenRect.getV0
  end
  
  def draw(p)
    startP=getPos(@startObject)
    endP=@pos unless @endObject
    endP=getPos(@endObject) if @endObject
    
    if @endObject and @startObject
	    if @moving==false and (@endObject.visible==false or @startObject.visible==false)
	      hide
	    end
    end
    white=AGColor.new(0xFF,0xFF,0xFF)
    white=AGColor.new(0xFF,0xFF,0x88) if @hovered
    black=AGColor.new(0,0,0)
    p.setLineWidth(1)
    p.drawArrow(startP,endP,5,white,black)
    middle=(endP+startP)*0.5
    
    p.renderText(AGStringUtf8.new("MUH"),middle,@@font)
  end
  
  def eventDragBy(e,v)
    super
    if @moving
      @pos=e.getMousePosition+(getRect.getV0-getScreenRect.getV0)
    end
    true
  end
  
  def eventMouseMotion(e)
    r=super
    p=@pos
    p=getPos(@endObject) if @endObject
    mp=e.getMousePosition-getScreenRect.getV0
    if getArrowTriangles(getPos(@startObject),p,10).select{|t|t.contains(mp)}.length>0
      puts "HOVER"
      @hovered=true
      return true
    else
      @hovered=false
    end
    r
  end
  def eventMouseButtonDown(e)
    r=super
    if @endObject.nil?
	    @moving=true
	    @pos=e.getMousePosition+(getRect.getV0-getScreenRect.getV0)
    else
      if @hovered
        @endObject=nil
        @moving=true
      end
    end    
    r
  end
  def eventMouseButtonUp(e)
    r=super
    if @moving
	    @moving=false
	    ts=getDragEnvironment.getAllDescendants.select{|w|w.is_a?(DragBox)}.select{|cell|
	      cell.getScreenRect.contains(e.getMousePosition)
	    }
	    if ts.length==0
	      hide
	    else
	      @endObject=ts[0]
	    end
    end
    
    r
  end
  def getDragEnvironment
    getAncestor(DragEnvironment)
  end
  def getDragGrid
    getAncestor(DragEnvironment).getAllDescendants.select{|c|c.is_a?(DragGrid)}[0]
  end
end


class AGWidgetWithConfig<AGWidget
  def initialize(p,r,ops)
    @options=ops
    super(p,r)
  end
  def getColor(name,default)
    v=@options[name]
    c=AGColor.new(v) if v=~/#[0-9A-Fa-f]{6}/
    c||=AGColor.new(default)
    c
  end
end

class ToolBar<AGWidgetWithConfig
  attr_reader :borderWidth
  def initialize(p,r,ops)
    super
    @ul=getColor("ul","#CCCCCC")
    @ur=getColor("ur","#CCCCCC")
    @ll=getColor("ll","#999999")
    @lr=getColor("lr","#999999")
    @borderWidth=ops["borderWidth"].to_i
    @count=0
  end
  def draw(p)
    p.drawGradient(getRect.origin,@ul,@ur,@ll,@lr)
  end
  def getNextChildRect
    cCount=getChildren.length
    #cCount=@count
    #@count+=1
    #p getChildRects,cCount
    #exit
    getChildRects[cCount].shrink(3)
  end
  
  def getChildRects
    rectSize=[width,height].min
    rects=[]
    0.upto(width/rectSize-1){|x|
      0.upto(height/rectSize-1){|y|
        rects<<AGRect.new(x*rectSize,y*rectSize,rectSize,rectSize)
      }
    }
    rects
  end
end

class ToolButton<AGButton
  def initialize(p,r,ops)
    puts "MUH1"
    puts self.object_id
    text=AGStringUtf8.new("HUP")
    super(p,r,text)
    setCaching(false)
    puts "MUH2"
    setRect(p.getNextChildRect)
    setCaption(text)
    setSurface(AGSurface::load(ops["caption-image"])) if ops["caption-image"]
    setName(ops["name"])
  end
end


class DragGrid<AGWidgetWithConfig
  attr_reader :cells, :cellWidth, :selected
  
  def initialize(p,r,options={})
    super
    @cellWidth=[options["cellWidth"].to_i,50].max
    @options=options
    @selected=nil
    @edit=options["edit"]
    @bgColor=getColor("bgColor","#7777FF")
    @borderColor=getColor("borderColor","#9999FF")
    @lines=(0..cellCountX).map{|x| AGLine2.new(AGVector2.new(x*@cellWidth,0),AGVector2.new(x*@cellWidth,height-1))}+
      (0..cellCountY).map{|y| AGLine2.new(AGVector2.new(0,y*@cellWidth),AGVector2.new(width-1,y*@cellWidth))}
      
    genCells()
  end
  
  def select(item)
    @selected.selected=false if @selected
    @selected=item
    if @selected
      @selected.selected=true
      checkEdit    
      @edit.setText(@selected.text)
    end
  end

  def textModified(e)
    @selected.text=@edit.getText
  end
  
  def draw(p)
    p.fillRect(getRect,@bgColor)
    @lines.each{|l|p.drawLine(l.getV0,l.getV1,@borderColor)}
  end
      
private
  def cellCountX
    width/@cellWidth
  end
  def cellCountY
    height/@cellWidth
  end
  def genCells()
    w=cellWidth
    (0..cellCountX).map{|x|
      (0..cellCountY).map{|y|
        AGRect2.new(x*w,y*w,w,w)
      }
    }.flatten.each{|c|newDragTarget(c)}
  end
  def newDragTarget(c)
    addChild(DragTarget.new(nil,c,@options,self))
  end
  def checkEdit
    
    unless @edit.is_a?(AGWidget)
      edit=getRoot.getAllDescendants.select{|w|w.getName==@edit}[0]
      unless edit.nil?
        @edit=edit
        pp @edit
        addHandler(@edit,:sigModified,:textModified)
      end
    end
  end
end


class DragSource<AGHoverWidget
  BORDER=15
  SECBORDER=5 #security
  def initialize(p,r,options)
    super(p,r)
    @font=AGFont.new("FreeSans.ttf",14)
    @textPos=AGVector2.new(20,20)
    @text=AGStringUtf8.new(options["text"]||"some Text")
    @mClass=findClass(options["class"])||DragBox
    @colors={false=>AGColor.new(0,0,0x77),true=>AGColor.new(0,0,0x88)}
    @borderColor=AGColor.new(0,0,0xAA)
  end
  def draw(p)
    r=getRect.origin.shrink(2)
    rad=10
    inrad=7
    p.setLineWidth(5)
    
    p.fillRoundRect(r,rad+inrad,@borderColor)
    p.fillRoundRect(r.shrink(inrad),rad,@colors[hovered])
    p.renderText(@text,@textPos,@font)
  end
  def eventMouseButtonDown(e)
    super
    if getScreenRect.shrink(BORDER+SECBORDER).contains(e.getMousePosition)
      # FIXME: dragenvironment is not == Screenspace evtl.
      getDragEnvironment.addChild(o=@mClass.new(getDragEnvironment,getScreenRect.shrink(BORDER),@text))
      o.eventMouseButtonDown(e)
      return true
    end
    
    false
  end
  def getDragEnvironment
    getAncestor(DragEnvironment)
  end
  private
  def findClass(name)
    cs=[]
    ObjectSpace.each_object{|o|cs<<o if o.to_s==name and o.is_a?(Class)}
    if cs.length==1
      cs[0]
    else
      nil
    end
  end
end


[DragGrid,DragSource,DragTrash,DragEnvironment,ToolBar,ToolButton,AppearEffect].each{|c|standardLayoutCreator(c)}

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

class CampaignEditorApp<AGApplication
  def initialize()
    super
    createSignal :sigFrame
    layout=AGLayout.new(nil)
    @layout=layout
    file=File.join('data','gui','layout','editor','campaign','main.xml')
    layout.loadXML( loadFile(file) )
    setMainWidget(layout)
    layout.setApp(self)
    if layout.getChild("bigTable")
	    layout.getChild("bigTable").modifyRow(1,10)
	    env=layout.getChild("dragEnvironment")
	    @grid=layout.getChild("dragGrid")
	    addHandler(env,:sigClick,:eventDeselect)
    end
    
    
  end
  def eventDeselect
    @grid.select(nil)
  end
  def eventFrame(t)
    sigFrame(t)
    super
  end
  
end

