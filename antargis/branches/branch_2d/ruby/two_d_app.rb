require 'terrain_2d.rb'

class Scene2D
	def getPickTriangles
		0
	end
	def getTriangles
		0
	end
	def getDrawnMeshes
		0
	end
end

class AntBaseMapView<AGApplication
	def initialize(w,h)
		super()
		@scene=Scene2D.new(w,h)
		@submain=nil
	end

	def draw
		@frame||=0
		@frame+=1

		#if (@frame%2)==0
			p=AGPainter.new
			@scene.setPainter(p)
			@scene.draw
			@scene.discardPainter
			p=nil
			GC.start
		#end
		super
	end
	def getScene
		@scene
	end
	def setCamera(p)
		@scene.setCamera(AGVector4.new(p.x,p.y,0))
	end
	def eventFrame(t)
	end

# 	def eventKeyDown(e)
# 		super
# 	end

	def eventMouseButtonDown(e)
		#raise 1
		case e.getButton
			when 1
				@mousePos=e.getMousePosition
		end
		@mayclick=true
		super
	end
	def eventMouseButtonUp(e)
		case e.getButton
			when 1
				@mousePos=nil
		end
		if @mayclick
			nodes=tryClick(e.getMousePosition)
			nodes.each{|n|
        puts "NODE: #{n}(#{n.class})"
        puts "NODE:"+(getMap.getEntity(n.node).to_s)
			}
			eventClick(nodes,e.getButton)
		end
		super
	end
	def eventMouseMotion(e)
		@mayclick=false
		if @mousePos
			diff=e.getMousePosition-@mousePos
			diff=diff*0.03
			@mousePos=e.getMousePosition
			p=@scene.getCamera.dim2+AGVector2.new(-diff[0],diff[1])
			@scene.setCamera(AGVector4.new(p[0],p[1],0,1))
			#queryRedraw
		end
		super
	end
	private
	def tryClick(pos)
		nodes=@scene.pick(pos.x,pos.y,1,1)	
	end
end

require 'map.rb'

class AntRubyMap
	def initTerrainMesh
		setTerrain(Terrain2D.new(getScene,self))
	end
end


require 'ant_models.rb'

module AntModels
	def AntModels.createModel(entityType,subType=nil,angle=nil)
		trace
		type=entityType.to_s
		if subType.to_s!=""
			type+="_"+subType.to_s
		end
		t=AGTexture.new(AGSurface.load("data/textures/2d/#{type}.png"))
		data=Mesh2DData.new(t)
		mesh=Mesh2D.new(getMap.getScene,data,AGVector4.new(0,0,0,1),0)
		mesh.setOrder(TREE_Z)
		mesh
	end
end


# some hacks
class Mesh2D
	def setRingColor(c)
	end
	def setAnimation(a)
	end
end