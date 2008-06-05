#!/usr/bin/env ruby

require 'ruby/antargislib.rb'



class MyApp<GLApp
  def initialize(w,h)
    super
    #node=Mesh.new(getScene,getMeshData("data/models/sack.ant2",0.3,"data/textures/models/sack.png"),AGVector4.new(0,0,0),0)
    node=Mesh.new(getScene,getMeshData("data/models/workshop.ant2",0.18,"data/textures/models/workshop.png"),AGVector4.new(0,0,0),-50)

    getScene.addNode(node)

    imp=AntImpostorData.new(node)

    tex=imp.getTexture
      
    image=AGImage.new(nil,AGRect2.new(0,0,tex.width,tex.height),tex,false)
    setMainWidget(image)
    
    getScene.removeNode(node)
  end

  def eventFrame(t)
    #imp=
    delay(100)
  puts t
    return true
  end

#    def draw(p)
#      p.blit(imp.getTexture)
#    end
end

#app=MyApp.new(128,128) #800,600)
app=MyApp.new(1024,768) #800,600)

app.run
