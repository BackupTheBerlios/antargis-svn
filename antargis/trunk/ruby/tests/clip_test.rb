#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

r=AGRect2.new(0,0,20,20)

class MyWidget<AGWidget
  def initialize(p,r)
    super
    @a=AGClipping.new
    @a.exclude(AGRect2.new(0,0,500,500))
    @a.include(AGRect2.new(100,100,200,200))
    @a.exclude(AGRect2.new(150,150,50,50))
  end
  def draw(p)
    rs=@a.clip(AGRect2.new(0,0,400,400))
    rs.each{|r|
      p.fillRect(r,AGColor.new(
        (rand*0xFF).to_i,
        (rand*0xFF).to_i,
        (rand*0xFF).to_i))
    }
  end
end

class MyApp<AGApplication
  def initialize
    super
    setMainWidget(MyWidget.new(nil,AGRect2.new(0,0,640,480)))
  end
  def eventFrame(t)  
    delay(100)
    true
  end
end
app=MyApp.new
app.run
