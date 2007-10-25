

class AntGrass<AntRubyEntity
	def initialize(map,high=false)
		super(map)
		setProvide("grass",true)
		if high
			@size=getRand*0.25+0.5
		else
			@size=getRand*0.25+0.25
		end
		setupMesh
	end
	def resourceChanged
		super
		setupMesh
	end
	
	
	private
	def setupMesh
		#return setMesh(@sze)
		if MyAntargislib.opengl
			setMesh(@size) #makeGrassMesh(@size))
		else
			puts "PROBLEM: NO GRASS WITHOUT GL!"
		end
	end
end

class AntHighGrass<AntGrass
	def initialize(map)
		super(map,true)
	end
end


class AntBush<AntRubyEntity
	def initialize(map)
		super
		setProvide("bush",true)
		@size=getRand*0.25+0.25
		setupMesh
	end
	def resourceChanged
		setupMesh
		super
	end
	
	private
	def setupMesh
		if MyAntargislib.opengl
			setMesh(@size) #makeBushMesh(@size*3))
		else
			puts "PROBLEM: NO GRASS WITHOUT GL!"
		end
	end
end


