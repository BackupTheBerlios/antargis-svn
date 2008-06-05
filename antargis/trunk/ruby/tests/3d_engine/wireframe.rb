#!/usr/bin/env ruby

$:.push("ext")

require 'antargis3dengine'
require 'antargisgui'

include Antargis3dengine
include Antargisgui

getMain.getVideo.initVideo(640,480,32,false,true)
app=AGApplication.new



class MWidget<AGGLWidget
  def initialize(p,r)
    super
    @scene=Scene.new(r.width.to_i,r.height.to_i)

    wireframe=Boa3dWireframe.new(@scene)

    a=AGVector3.new(0,0,0)
    b=AGVector3.new(10,0,0)
    white=AGVector4.new(1,1,1,1)

    wireframe.addLine(a,b,white)

    @scene.addNode(wireframe)

  end
  def drawGL
    @scene.draw
  end

  
end

widget=MWidget.new(nil,AGRect2.new(0,0,640,480))

app.setMainWidget(widget)

app.run



