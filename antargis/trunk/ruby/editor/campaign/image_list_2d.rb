class ImageList2D<AGWidget
  
  def initialize(p,r,options)
    super(p,r)
    
    createSignal :sigSelected
    dir="data/gui/campaign"
    files=getDirectory(dir).select{|f|f=~/\.png$/}.uniq
    pp files
    #exit
    @images=files.map{|file|[file,AGTexture.new(AGSurface::load(File.join(dir,file)))]}*3
    assert{@images.length>0}
    @selected=2
    @border=AGBorder.new("button.border.normal")
    @pos=0
    
    @imageHeight=height*0.7
    @imageWidth=@imageHeight*1.3
    @imageY=(height-@imageHeight)/2
    @margin=20
    @selected=2
    @color=AGColor.new(0,0,0)
  end
  
  def getImageByFilename(filename)
    @images.select{|pair|pair[0]==filename}[0][1]
  end
  
  def getFirstImage
    @images[0][0]
  end
  
  def eventMouseButtonDown(e)
    super
    true
  end
  
  def eventMouseClick(e)
    pos=e.getRelMousePosition
    if getRect.origin.contains(pos)
      i=0
      getRects.each{|r|
        if r.contains(pos)
          @selected=i
          sigSelected(*@images[i])
          return true
        end
        i+=1
      }
      return true
    end
    super
  end
  
  def draw(p)
    i=0
    getRects.each{|r|
      imagePair=@images[i]
      file,image=imagePair
      from=image.getRect
      p.setLineWidth(1)
      if i==@selected
        p.fillRect(r.grow(5),@color)
        p.drawRect(r.grow(2),AGColor.new(0xFF,0xFF,0xFF))
      end
      p.blit(image,r,from)
      
      i+=1
    }
  end
  
  def getRects
    (1..@images.length).map{|x|@pos+(x-1)*(@imageWidth+@margin)}.map{|x|AGRect2.new(x,@imageY,@imageWidth,@imageHeight)}
  end
  
end

[ImageList2D].each{|c|standardLayoutCreator(c)}
