#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

getMain.initVideo(800,600,32,false,false)



class MyWidget<AGWidget
  def initialize(p,r)
    super

a=AGSurface.new(64,64)
p=AGPainter.new(a)

p.fillRect(AGRect2.new(10,10,10,10),AGColor.new(0xFF,0,0))
p=nil
a.save("test0.png")
  @t1=AGTexture.new(a)


t=AGTexture.new(64,64)
p=AGPainter.new(t)

p.fillRect(AGRect2.new(10,10,10,10),AGColor.new(0xFF,0,0))
p=nil
s=AGSurface.new(t.sdlTexture)
s.save("test1.png")


  @t2=AGTexture.new(s)

    @t3=AGTexture.new(64,64)
    @t3.beginPaint
    @t3.blit(@t2,AGRect2.new(0,0,64,64),AGRect2.new(0,0,64,64))
    @t3.endPaint
s=AGSurface.new(@t3.sdlTexture)
s.save("test2.png")

  end
  def draw(p)
    #p.
    p.fillRect(AGRect2.new(0,0,256,256),AGColor.new(0,0xFF,0))
    p.blit(@t1,AGRect2.new(0,0,64,64))
    p.blit(@t2,AGRect2.new(64,0,64,64))
    p.blit(@t3,AGRect2.new(128,0,64,64))
  end
end



app=AGApplication.new

imw=MyWidget.new(nil,AGRect2.new(0,0,800,600))

app.setMainWidget(imw)
app.run
