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
    
    @children=[]
  end
  def draw(p)
    p.drawGradient(getRect.origin,@ul,@ur,@ll,@lr)
  end
  
  def arrangeChildren
    count=0
    @children.each{|child|
      many=1
      many=child.boxWidth if child.respond_to?(:boxWidth)
      pp count,many
      r=getChildRect(count,many)
      p r
      child.setRect(r)
      count+=many
      child.queryRedraw
    }
  end
  def addChild(c)
    super
    @children<<c
    arrangeChildren
  end
  
  private
  def getChildRect(pos,many)
    rects=getChildRects
    r=rects[pos]
    pos+=1
    many-=1
    while many>0
      r=AGRect2.new(r.getV0,rects[pos].getV1)
      pos+=1
      many-=1
    end
    r.shrink(3)
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
    text=AGStringUtf8.new("HUP")
    super(p,r,text)
    setCaching(false)
    setCaption(text)
    setSurface(AGSurface::load(ops["caption-image"])) if ops["caption-image"]
    setName(ops["name"])
  end
end

class ToolEdit<AGEdit
  attr_reader :boxWidth
  def initialize(p,r,ops)
    super(p,r)
    setMulti(false)
    setVAlign(EDIT_VCENTER)
    setText(AGStringUtf8.new(ops["text"]))
    setMutable(ops["mutable"]!="false")
    
    @boxWidth=3
  end
  def eventGotFocus
    self.boxWidth=5
  end
  def eventLostFocus
    self.boxWidth=3
  end
  def boxWidth=(w)
    @boxWidth=w
    getAncestor(ToolBar).arrangeChildren
  end
end

class ToolCombo<AGComboBox
  def initialize(p,r,ops)
    super(p,r)
  end
  def boxWidth
    3
  end
end
