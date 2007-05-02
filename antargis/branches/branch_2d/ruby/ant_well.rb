

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
		setMesh
	end


end