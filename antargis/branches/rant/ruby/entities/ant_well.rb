

class AntWell<AntRubyEntity
	def initialize(map)
		super
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
		setMesh
	end


end