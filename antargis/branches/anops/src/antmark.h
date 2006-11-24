/*

%markfunc AntsMap "AntMap_markfunc"
%exception AntMap::AntMap {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//	rb_gc_register_address(self);
	//printf("setting mruby:widget\n");
    }

//%markfunc AntEntity "AntEntity_markfunc"
%exception AntEntity::AntEntity {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }

%markfunc AntMap "AntMap_markfunc";
%markfunc AntEntity "AntEntity_markfunc";


%exception SceneNode::SceneNode {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
    }

%exception Mesh::Mesh {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//	printf("%X\n",self);
    }
%exception MeshData::MeshData {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//	printf("%X\n",self);
    }
%exception AnimMesh::AnimMesh {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//	printf("%X\n",self);
    }
%exception AnimMeshData::AnimMeshData {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//	printf("%X\n",self);
    }
%exception TerrainMesh::TerrainMesh {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
    }
%exception Smoke::Smoke {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
    }

%markfunc Scene "Scene_markfunc";
%markfunc GLApp "GLApp_markfunc";
%markfunc AnimMesh "AnimMesh_markfunc";
%markfunc Mesh "Mesh_markfunc";

*/
%markfunc GLApp "GLApp_markfunc";
