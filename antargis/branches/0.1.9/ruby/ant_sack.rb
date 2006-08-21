
class AntSack<AntAnimal
	def initialize(p=AGVector2.new(0,0))
		super
		mp=AGVector3.new(0,0,0)
		mesh=Mesh.new(getMap.getScene,getMeshData("data/models/sack.ant2",0.3,"data/textures/models/sack.png"),AGVector4.new(0,0,0),0)
		setMesh(mesh)
		#setPos(AGVector2.new(p.x,p.y))
		@enabled=true
		@storeGood=["food","tool","corn","stone","wood"]
	end
	def eventDie
		getMap.removeEntity(self)
	end
	def resourceChanged
		@storeGood.each{|r|
			setProvide(r,resource.get(r)>0)
		}
	end
end