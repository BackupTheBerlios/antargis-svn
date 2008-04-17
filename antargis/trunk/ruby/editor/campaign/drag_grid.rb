require 'ruby/antargislib.rb'
require 'pp'

module Antargis
  class AGWidget
    def getRoot
      return self if getParent.nil?
      getParent.getRoot
    end
    def getAllDescendants
      getChildren.map{|child|[child]+child.getAllDescendants}.flatten
    end
  end
end

class GenericLayoutCreator<AGLayoutCreator
  def initialize(c)
    super()
    @c=c
  end
  def create(p,r,node)
    setResult @c.new(p,r)
  end
end

def standardLayoutCreator(pClass)
  name=pClass.to_s
  name=name[0..0].downcase+name[1..-1]
  creator=GenericLayoutCreator.new(pClass)
  getLayoutFactory.addCreator(name,creator)
end


class DragObject<AGWidget
  def initialize(p,r)
    super
    @dragging=false
    @lastCell=nil
  end
  def draw(p)
    c=AGColor.new(0x77,0x77,0x77)
    p.fillRect(getRect.origin,c)
  end
  def eventDragBy(e,v)
    super
    if @dragging
    #pp v.x,v.y
      setRect(getRect+v)
    end
    #@dragging=true
    true
  end
  def eventMouseButtonDown(e)
    r=super
    sRect=getScreenRect
    if sRect.contains(e.getMousePosition)
      @dragging=true
      root=getRoot
      getParent.removeChild(self)
      root.addChild(self)
      setRect(sRect)
    end
    r
  end
  def eventMouseButtonUp(e)
    g=getRoot.getAllDescendants
    pp g
    g=g.select{|w|w.is_a?(DragGrid)}
    pp g
    grid=g[0]
    cell=grid.getCell(getMiddle)
    puts cell
    if cell
      if grid.cellIsFree(cell)
	      getParent.removeChild(self)
	      grid.addChild(self)
        @lastCell=cell
	      setRect(cell.shrink(5))
      elsif @lastCell
        setRect(@lastCell)
	    else
	      hide
	    end
        
    else
	    hide
    end
    @dragging=false
    true
  end
  def getMiddle
    getScreenRect.getMiddle
  end
end

class DragGrid<AGWidget
  CELL_COUNT_X=10
  CELL_COUNT_Y=10
  def draw(p)
    p.fillRect(getRect.origin,AGColor.new(0x77,0x77,0xFF))
    lineStart=AGVector2.new(0,0)
    cells.each{|c|drawCell(p,c)}
  end
  def cellWidth
    100
  end
  def cells
    w=cellWidth
    (0..CELL_COUNT_X).map{|x|
      (0..CELL_COUNT_Y).map{|y|
        AGRect2.new(x*w,y*w,w,w)
      }
    }.flatten
  end
  def drawCell(p,cell)
    p.setLineWidth(1) 
    c1=AGColor.new(0x33,0x33,0xFF)
    c2=AGColor.new(0x11,0x11,0xFF)
    p.drawRect(cell,c1)
  end
  def getCell(v)
    cs=cells.select{|c|(c+getScreenRect.getV0).shrink(10).contains(v)}
    cs[0]
  end
end

class DragSource<AGWidget
  def initialize(p,r)
    super
  end
  def draw(p)
    p.fillRect(getRect.origin,AGColor.new(0,0,0x77))
  end
  def eventMouseButtonDown(e)
    super
    if getScreenRect.contains(e.getMousePosition)
      getRoot.addChild(o=DragObject.new(nil,getScreenRect.shrink(5)))
      o.eventMouseButtonDown(e)
      return true
    end
    
    false
  end
end

class DragSources<AGWidget
  def initialize(p,r)
    super
    cells.map{|r| DragSource.new(self,r)}.each{|ds|addChild(ds)}
  end
  def cellCount
    2
  end
  def cells
    (0...cellCount).map{|y|
      AGRect2.new(0,y*cellSize,cellSize,cellSize).shrink(2)
    }
  end    
  def cellSize
    104
  end
end

[DragGrid,DragSources].each{|c|standardLayoutCreator(c)}


app=AGApplication.new
layout=AGLayout.new(nil)
layout.loadXML(loadFile("data/gui/layout/editor/campaign/main.xml"))
app.setMainWidget(layout)

app.run