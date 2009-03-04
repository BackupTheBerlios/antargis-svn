require 'ruby/antargislib.rb'
require 'pp'

require "./dialogs.rb"
require "./toolbar.rb"

require './ruby_tools.rb'
require './story_editor.rb'
require './ruby_signals.rb'
require './campaign_data.rb'

#setDebugLevel(0)

# TODO:
# * put all draggable-objects within context of DragEnvironemt

#setDebugLevel(0)

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
    ps=createArrowPolies(from,to,width)
    ts=[ps[0][0..2],ps[0][1..3],ps[1]]
    r=ts.map{|t|AGTriangle2.new(t[0],t[1],t[2])}
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
    def getChildByType(type)
      getAllDescendants.select{|c|c.is_a?(type)}[0]
    end

    def getWidgetTree
      [self,getChildren.map{|c|c.getWidgetTree}]
    end
    def focused?
      if getParent
        getParent.hasFocus(self)
      else
        true
      end
    end
  end
end


class AGHoverWidget<AGWidget
  attr_accessor :hoverBorder
  def initialize(*s)
    super
    @hoverBorder=5
  end
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

module DragObject
  def canFocus
    false
  end
end

class DragBox<AGHoverWidget
  BORDER_WIDTH=15
  
  include DragObject
  
  attr_accessor :text, :grid
  attr_accessor :selected

  
  def initialize(p,r,text,dragging=true)
    super(p,r)
    @dragging=dragging
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
    if hovered or @dragging
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
    @dragging=true
    moveToContext(getDragEnvironment)
    setRect(getRect.shrink(5))
  end
  
  def eventMouseButtonDown(e)
    r=super
    sRect=getRect
    if sRect.contains(e.getRelMousePosition)
      dragGrid=getDragGrid
      if sRect.shrink(BORDER_WIDTH).contains(e.getRelMousePosition)
        startDragging
        r=true
      elsif @lastCell
        startLine(e)
        r=true
      end
      
      #dragGrid.select(self) if dragGrid
    end
    r
  end
  
  def startLine(e)
    env=getAncestor(DragGrid)
    env.addChild(line=DragLine.new(env,env.getRect,self))
    line.setButtonDown(true,e.getMousePosition) # enable dragging
    line.startDragging(e)
    #line.eventMouseButtonDown(e)
  end
  
  def eventMouseButtonUp(e)
    return super unless @dragging
    #moveToContext()

    if false
      cells=getDragEnvironment.getDragTargets
      scells=cells.select{|c|c.getScreenRect.shrink(10).contains(getMiddle)}
      cell=scells[0]
    else
      env=getDragEnvironment.getChildByType(DragGrid)
      cell=env.cells.select{|c|c.screenRect.shrink(10).contains(getMiddle)}[0]
    end
    
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
      dragGrid=getDragGrid
    dragGrid.select(self) if dragGrid
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
    
    moveToContext(getDragGrid)
    setRect(cell.rect.shrink(5))
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

class DragBoxLevel<DragBox
  attr_accessor :filename
end

class DragBoxStory<DragBox
  attr_accessor :story
  def initialize(p,r,text,dragging=true)
    super
    @color=AGColor.new(0xFF,0xAA,0)
  end
end

class DragLine<AGWidget
  include Arrows
  
  attr_accessor :selected, :text
  attr_writer :endObject
  
  def initialize(p,r,startObject)
    super(p,r)
    @startObject=startObject
    @endObject=nil
    @pos=nil
    @@font||=AGFont.new("FreeSans.ttf",14)
    @@font.setColor(AGColor.new(0,0,0))
    @hovered=false
    @text=AGStringUtf8.new("none")
  end
    
  def getPos(obj)
    obj.getScreenRect.getMiddle-getScreenRect.getV0
  end
  
  def draw(p)
    startP=getPos(@startObject)
    if @endObject
      endP=getPos(@endObject)
    else
      endP=@pos
    end
   
    if @endObject and @startObject
      if @moving==false and (@endObject.visible==false or @startObject.visible==false)
        #hide
        getParent.removeChild(self)
        return
      end
    end
    white=AGColor.new(0xFF,0xFF,0xFF)
    white=AGColor.new(0xFF,0xFF,0x88) if @hovered
    black=AGColor.new(0,0,0)
    p.setLineWidth(1)
    assert{endP}
    p.drawArrow(startP,endP,5,white,black)
    middle=(endP+startP)*0.5
    
    p.renderText(AGStringUtf8.new(@text),middle,@@font)
  end
  
  def eventDragBy(e,v)
    super
    if @moving
      @pos=e.getMousePosition+(getRect.getV0-getScreenRect.getV0)
    end
    true
  end
  
  def startDragging(e)
    @moving=true
    @pos=e.getMousePosition+(getRect.getV0-getScreenRect.getV0)
  end
  
  def eventMouseMotion(e)
    r=super
    if @endObject
      p=getPos(@endObject)
    else 
      p=@pos
    end
    mp=e.getMousePosition-getScreenRect.getV0
    if getArrowTriangles(getPos(@startObject),p,10).select{|t|t.contains(mp)}.length>0
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
        @pos=e.getMousePosition+(getRect.getV0-getScreenRect.getV0)
        r=true
      end
    end    
    getDragGrid.select(self) if getDragGrid
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

Cell=Struct.new(:screenRect,:rect,:node)
class Cell
  def free
    @node.nil?
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
      getApp.startEffect("showEdit")
      @edit.gainCompleteFocus
    else
      getApp.startEffect("hideEdit")
    end
  end

  def textModified(e)
    @selected.text=@edit.getText
  end
  
  def draw(p)
    p.fillRect(getRect,@bgColor)
    @lines.each{|l|p.drawLine(l.getV0,l.getV1,@borderColor)}
  end
  
#  def eventMouseButtonDown(e)
 #   return super
  #end
  
  def addChild(c)
    super
    getChildren.sort{|b,a|a.is_a?(DragLine)<=>b.is_a?(DragLine)}.each{|child|
      addChildBack(child)
    }    
    
  end
      
private
  def cellCountX
    width/@cellWidth
  end
  def cellCountY
    height/@cellWidth
  end
  def genCells()
    @cells=[]
    w=cellWidth
    (0..cellCountX).map{|x|
      (0..cellCountY).map{|y|
        AGRect2.new(x*w,y*w,w,w)
      }
    }.flatten.each{|c|newDragTarget(c)}
  end
  def newDragTarget(c)
    #addChild(DragTarget.new(nil,c,@options,self))
    @cells << Cell.new(toScreen(c),c,nil)
  end
  def checkEdit
    
    unless @edit.is_a?(AGWidget)
      edit=getRoot.getAllDescendants.select{|w|w.getName==@edit}[0]
      unless edit.nil?
        @edit=edit
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
      o.setName("DRAG")
      o.setButtonDown(true,e.getMousePosition)
      o.startDragging
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


[DragGrid,DragSource,DragEnvironment,ToolBar,ToolButton,ToolEdit,ToolCombo,AppearEffect,HideEffect].each{|c|standardLayoutCreator(c)}

