#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

getMain.initVideo(800,600,32,false,false)

def makeExampleMesh(scene,type)
  t=AGTexture.new(AGSurface.load("data/textures/2d/#{type}.png"))
  data=Mesh2DData.new(t)
  mesh=Mesh2D.new(scene,data,AGVector4.new(0,0,0,1),0)
  mesh.setOrder(TREE_Z)
  mesh
end

class MyTerrain #<TerrainBase
  def initialize(scene,map)
    #super
    @scene=scene

    createTiles
  end

  def createTiles
    @textures={
      :grass=>AGTexture.new(AGSurface.load("data/textures/2d/terrain/basic_grass.png")),
      :grass2=>AGTexture.new(AGSurface.load("data/textures/2d/terrain/basic_grass2.png"))
    }
    @meshdata={}
    @textures.each{|k,v|
      @meshdata[k]=Mesh2DData.new(v)
    }

    (0..10).each{|x|
      (0..100).each{|y|
        ix=x*2+(y % 2)
        iy=y
        ix*=64
        iy*=32
        v=AGVector4.new(ix,iy,0,1)
        t=:grass
        if x+y>5
          t=:grass2
        end
        mesh=Mesh2D.new(@scene,@meshdata[t],v,0)
        @scene.addNode(mesh)
        mesh.setOrder(TERRAIN_Z)
      }
    }
  end
end


class My2dWidget<AGWidget
  def initialize(p,r)
    super
    @scene=Scene2D.new(r.width.to_i,r.height.to_i)
    mesh=makeExampleMesh(@scene,:farm)
    mesh.setPos(AGVector3.new(130,50,0))
    @scene.addNode(mesh)
    mesh=makeExampleMesh(@scene,:tree)
    mesh.setPos(AGVector3.new(300,50,0))
    @scene.addNode(mesh)

    @terrain=MyTerrain.new(@scene,nil)
  end
  def draw(p)
    @scene.setPainter(p)
    @scene.draw
    @scene.discardPainter
  end

  def eventMouseButtonDown(e)
    #raise 1
    @mousePos=e.getMousePosition
    super
  end
  def eventMouseButtonUp(e)
    @mousePos=nil
    super
  end
  def eventMouseMotion(e)
    if @mousePos
      diff=e.getMousePosition-@mousePos
      @mousePos=e.getMousePosition
      p=@scene.getCamera.dim2+AGVector2.new(-diff[0],diff[1])
      @scene.setCamera(AGVector4.new(p[0],p[1],0,1))
      queryRedraw
    end
    super
  end

#   def prepareDraw
#     queryRedraw
#   end

end

class My2dTest<AGApplication
  def initialize
    super
    setMainWidget(My2dWidget.new(nil,AGRect2.new(0,0,getMain.width,getMain.height)))
  end
  def eventFrame(t)
    
    delay(20)
    super
  end
end

app=My2dTest.new

app.run
