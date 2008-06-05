
# the AntNameDisplay shows the name of a hero some place above the actual hero
# mesh. It consists of a button with a custom text-display. 
# It checks if the hero's player' changed in each frame. TODO: here's some possiblity for optimization.
# Enemy heroes' names are displayed in red - your own heroes are display in yellow.
# the position on the screen must be given from outside (from the application e.g.)
class AntNameDisplay<AGWidget
  @@fontChangeCount=0

  # p - the parent widget
  # r - the surrounding rectangle
  # hero - the inspected hero
  # map - the containing map-object
  def initialize(p,r,hero,map)
    super(p,r)
    @map=map
    @hero=hero
    @font=getTheme.getFont("heroName.font")
    @oldfont=@font
    @name=@hero.getName
    addChild(@mb=AGButton.new(self,AGRect.new(0,0,width,height),_("")))
    
    # disable button - so it can't be clicked anymore (no hovering either)
    @mb.setEnabled(false)
    addChild(@textWidget=AGText.new(self,AGRect.new(0,0,width,height),AGStringUtf8.new(@hero.getName),@font))
    
    # assign font-types
    @fonts={true=>getTheme.getFont("heroName.font"),false=>getTheme.getFont("enemyHero.font")}
    
    # enable caching of widget texture - this improves drawing performance (slightly)
    setCaching(true)
  end

  # enforce integer position  
  # 
  def setRect(r)
    super(AGRect.new(r.x.to_i,r.y.to_i,r.w.to_i,r.h.to_i))
  end
  
  def getText
    @name
  end

  def prepareDraw
    # check if @hero's player changed
    @font=@fonts[@hero.getPlayer==@map.getPlayer]
    if @font!=@oldfont
      # this is the case - so assign new font to textWidget
      @oldfont=@font
      @textWidget.setFont(@font)
      # query redraw of (possibly) cached widget texture
      queryRedraw
    end
    super
  end
end
