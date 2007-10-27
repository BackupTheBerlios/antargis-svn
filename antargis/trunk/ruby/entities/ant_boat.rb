def getBoatMeshData
	getMeshData("data/models/boat_simple.ant2",0.12,"data/textures/models/boat_simple.png")
end

# A boat lying around - for rowing this is not used!
class AntBoat<AntRubyEntity
	def initialize(map)
		super
		setupMesh
	end
private
	def setupMesh
		setMesh
	end
end