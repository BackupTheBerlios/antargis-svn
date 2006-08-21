

class AntWell<AntRubyEntity
	def initialize
		super(AGVector2.new(0,0))
		@type=3
		setProvide("water",true)
		@defeated=[]
		@atHome=[]
		@lastBirth=0
		setDirection(-30)
		resource.set("water",1000)
		setupMesh
	end
	
	def setupMesh
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/well.ant2",0.06,"data/textures/models/well.png"),AGVector4.new(0,0,0),-30)
		#mesh=Mesh.new(getMap.getScene,getMeshData("data/models/townhall.ant2",3.2,"data/textures/models/townhall.png"),AGVector4.new(0,0,0),-30)
		setMesh(mesh)
	end


end