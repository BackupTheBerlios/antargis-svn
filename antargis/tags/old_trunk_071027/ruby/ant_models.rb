
$useImpostors=false

def getStaticModelRotation(name)
	rot={
	}

	if rot.member?(name)
		return rot[name]
	end
	return 0
end

def getStaticModelScaling(name)
	scales={
		"sack"=>0.3,
		"stub"=>0.04,
	}

	if scales.member?(name)
		return scales[name]
	end
	return 1
end

def createModel(entityType,subType=nil,angle=nil)
	mesh=nil

	animMeshes={
		:man=>{
			["wood"]=>"data/models/man_e_wood.anim",
			["stone","flour","corn"]=>"data/models/man_e_stones.anim",
			["walk","sitdown","sit"]=>"data/models/man_e_walk.anim",
			["fight_dagger"]=>"data/models/man_e_dagger.anim",
			["fight_shield"]=>"data/models/man_e_shield.anim",
			["fight_sword"]=>"data/models/man_e_sword.anim",
			["fight_bow"]=>"data/models/man_e_bow.anim"
		},
		:sack=>{
			[]=>"sack"
		},
		:tree=>{
			["stub"]=>"stub",
			[0]=>["data/models/fir2.ant2",0.45,"data/textures/models/fir5.png",false],
			[1]=>["data/models/fir2.ant2",0.45,"data/textures/models/fir7.png",false],
			[2]=>["data/models/tree5.ant2",0.45,"data/textures/models/tree3.png",false],
			[3]=>["data/models/tree5.ant2",0.45,"data/textures/models/tree5.png",false],
			[4]=>["data/models/tree5.ant2",0.45,"data/textures/models/tree9.png",false],
			[5]=>["data/models/tree6.ant2",0.45,"data/textures/models/tree5.png",false],
			[6]=>["data/models/tree5.ant2",0.45,"data/textures/models/tree10.png",false],
			[7]=>["data/models/tree_simple1.ant2",0.3,"data/textures/models/tree_simple1.png"],
			[8]=>["data/models/tree_simple2.ant2",0.3,"data/textures/models/tree_simple1.png"],
			[9]=>["data/models/tree_simple5.ant2",0.3,"data/textures/models/tree_simple5.png"]
		}

	}

	map=animMeshes[entityType]

	map.each{|k,v|
		if k.member?(subType) or k.length==0
			mesh=v
		end
	}
	scenenode=nil
	if mesh
		if mesh.is_a?(Array)
			ant2name=mesh[0]
			scale=mesh[1]
			pngname=mesh[2]
			culling=true
			if mesh.length>3
				culling=mesh[3]
			end
			data=getMeshData(ant2name,scale,pngname)
			data.setCulling(culling)
			data.setTransparent(true)
			angle||=getStaticModelRotation(name)
			scenenode=Mesh.new(getMap.getScene,data,AGVector4.new(0,0,0),angle)
					
		elsif mesh=~/anim$/
			scenenode=AnimMesh.new(getMap.getScene,getAnimMeshData(mesh))
		else
			ant2name="data/models/"+mesh+".ant2"
			pngname="data/textures/models/"+mesh+".png"
			name=mesh
			angle||=getStaticModelRotation(name)
			scenenode=Mesh.new(getMap.getScene,getMeshData(ant2name,getStaticModelScaling(name),pngname),AGVector4.new(0,0,0),angle)
		end
	end

	if $useImpostors
		# FIXME:replace by impostor
		if entityType!=:sack
			return createModel(:sack)
		end
	end

	return scenenode
end