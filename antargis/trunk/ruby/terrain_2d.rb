class Terrain2DTile<Mesh2D
end

class Terrain2D<TerrainBase
  def initialize(scene,map)
    super
    @scene=scene
    @map=map
    createTiles
  end

  def createTiles
    @textures={
      :grass=>AGTexture.new(AGSurface.load("data/textures/2d/terrain/full_grass.png")),
      :grass2=>AGTexture.new(AGSurface.load("data/textures/2d/terrain/full_grass2.png")),
      :water=>AGTexture.new(AGSurface.load("data/textures/2d/terrain/full_water.png"))
    }
    @meshdata={}
    @textures.each{|k,v|
      @meshdata[k]=Mesh2DData.new(v)
    }

    (0..(@map.getW/4-2)).each{|x|
      (0..(@map.getH/2-1)).each{|y|
        ix=x
        iy=y
        ix*=4
        iy*=2
        v=AGVector4.new(ix,iy,0,1)
        t=:grass
        if @map.getHeight(ix+2,iy+1)<0
          t=:water
        end
#         if x+y>5
#           t=:grass2
#         end
        mesh=Terrain2DTile.new(@scene,@meshdata[t],v,0)
        @scene.addNode(mesh)
        mesh.setOrder(TERRAIN_Z)
      }
    }
  end
end
