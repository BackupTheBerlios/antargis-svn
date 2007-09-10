class AntNPC<AntRubyEntity
	attr_accessor :npcType
	def initialize()
		super(AGVector2.new(0,0))
		setProvide("NPC",true)
		@npcType="smith" #druid"
		setupMesh
	end
	def resourceChanged
		super
		setupMesh
	end
	
	def saveXML(node)
		node.set("npcType",@npcType)
		super
	end
	def loadXML(node)
		@npcType=node.get("npcType")
		super
		setupMesh
	end
	
	def setupMesh
		case @npcType
			when "smith"
				setMesh(:smith)
	 			#setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/smith_lp.ant2",0.08,"data/textures/models/smith_lp.png"),AGVector4.new(0,0,0,0),0))
			else
				setMesh(:druid)
	 			#setMesh(Mesh.new(getMap.getScene,getMeshData("data/models/druid_lp.ant2",0.08,"data/textures/models/druid_lp.png"),AGVector4.new(0,0,0,0),0))
		end
	end
end
