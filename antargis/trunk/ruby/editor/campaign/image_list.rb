class ImageList<AGWidget
  def initialize(p,r,options)
    super(p,r)
    dir="data/gui/campaign"
    files=getDirectory(dir).select{|f|f=~/\.png$/}.uniq
    #exit
    @images=files.map{|file|[file,AGTexture.new(AGSurface::load(File.join(dir,file)))]}
    assert{@images.length>0}
    @selected=2
    @border=AGBorder.new("button.border.normal")
    @pos=0
  end
  
  def eventMouseMotion(e)
    @pos=1-e.getMousePosition.x/width
    super
  end
  
  def draw(p)
    rs=getRects
    
    
    0.upto(rs.length-1){|i|
      t=@images[3][1]
      r=rs[i]+getScreenRect.getV0
      f=t.getRect
      pos=i.to_f/(rs.length)*0.5+@pos-1
      trisFrom=makeTriangles(f,AGTriangle2)
      #trisTo=makeTriangles(r,AGTriangle3)
      trisTo=makeTriangle(getScreenRect,pos,f)
      (0..1).each{|j|
        p.getTarget.blit3dTri(t,trisFrom[j],trisTo[j])
      }
      
    }    
  end
  
  def makeTriangle(drawRect,pos,fromRect)
    pos01=(pos+1)*0.5
    
    height=drawRect.height*0.7
    width=fromRect.width/fromRect.height*height
    
    centerx=drawRect.x+drawRect.width*pos01
    lx=centerx-width*0.5
    rx=centerx+width*0.5
    f=0.5
    y0=y1=height*f
    f=0.3
    dl=dr=0
    
    dr=pos if pos<0
    dl=-pos if pos>0
    
    d1=[0,[y1,-height*(pos)*f].min].max
    d0=[0,[y0,height*(pos)*f].min].max
    
    y1-=d1
    y0-=d0
    
    y=drawRect.getMiddle.y
    
    vs=[AGVector3.new(lx,y+y0,dl),
      AGVector3.new(lx,y-y0,dl),
      AGVector3.new(rx,y-y1,dr),
      AGVector3.new(rx,y+y1,dr),
       ]
    [AGTriangle3.new(vs[0],vs[1],vs[2]),AGTriangle3.new(vs[0],vs[2],vs[3])]
  end
  
  def makeTriangles(r,type)
    a=[r.getV0,r.getV10,r.getV1,r.getV01]
    a=a.map{|v|AGVector3.new(v,0.0)} if type==AGTriangle3
    [type.new(a[0],a[1],a[2]),type.new(a[0],a[2],a[3])]
  end
  
  def getRects 
    @rects||=genRects(@images.length)
  end
  
  def genRects(count)
    getRect.origin.tile(count,1).map{|r|r.shrink(5)}
  end
  
end

[ImageList].each{|c|standardLayoutCreator(c)}
