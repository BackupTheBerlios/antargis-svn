#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

getMain.initVideo(800,600,32,false,false)


class MyWidget<AGWidget
  def initialize(p,r)
    super
    @x=0
    @y=0
  end
  def prepareDraw
  end
  def draw(p)
    #p.fillRect(AGRect2.new(0,0,300,300),AGColor.new(0,0,0))
    p.fillRect(AGRect2.new(@x,@y,10,10),AGColor.new(0xFF,0,0))
  end
  def eventFrame(t)
    @x+=2
    @y+=1
    @x%=207
    @y%=293
    #queryRedraw
    pushChangeRect(AGRect2.new(@x-2,@y-2,12,12))
  end
end

class MyApp<AGApplication
  def initialize
    super
    @imw=MyWidget.new(nil,AGRect2.new(0,0,800,600))
    setMainWidget(@imw)
  end
  def eventFrame(t)
    @imw.eventFrame(t)
    delay(5)
    true
  end
end

app=MyApp.new
app.run
