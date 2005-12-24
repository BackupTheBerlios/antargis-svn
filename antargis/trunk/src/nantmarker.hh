%exception AnimMeshData::AnimMeshData {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc AnimMeshData "general_markfunc"
%exception WaterPiece::WaterPiece {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc WaterPiece "general_markfunc"
%exception Scene::Scene {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc Scene "general_markfunc"
%exception AnimMesh::AnimMesh {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc AnimMesh "general_markfunc"
%exception NewDecal::NewDecal {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc NewDecal "general_markfunc"
%exception Mesh::Mesh {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc Mesh "general_markfunc"
%exception RubyObject::RubyObject {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc RubyObject "general_markfunc"
%exception Smoke::Smoke {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc Smoke "general_markfunc"
%exception MeshData::MeshData {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc MeshData "general_markfunc"
%exception GLTree::GLTree {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc GLTree "general_markfunc"
%exception SceneNode::SceneNode {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc SceneNode "general_markfunc"
%exception TerrainPieceVA::TerrainPieceVA {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc TerrainPieceVA "general_markfunc"
%exception HeightMap::HeightMap {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc HeightMap "general_markfunc"
%exception AntMap::AntMap {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc AntMap "general_markfunc"
%exception AntEntity::AntEntity {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc AntEntity "general_markfunc"
%typemap(out) AnimMeshData*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AnimMeshData,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) WaterPiece*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_WaterPiece,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) Scene*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Scene,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) AnimMesh*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AnimMesh,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) NewDecal*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_NewDecal,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) Mesh*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<GLTree*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_GLTree,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Mesh,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) Smoke*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Smoke,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) MeshData*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_MeshData,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) SceneNode*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AnimMesh*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AnimMesh,0);
else if(dynamic_cast<WaterPiece*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_WaterPiece,0);
else if(dynamic_cast<NewDecal*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_NewDecal,0);
else if(dynamic_cast<Smoke*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Smoke,0);
else if(dynamic_cast<MeshData*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_MeshData,0);
else if(dynamic_cast<GLTree*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_GLTree,0);
else if(dynamic_cast<TerrainPieceVA*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_TerrainPieceVA,0);
else if(dynamic_cast<Mesh*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Mesh,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_SceneNode,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) GLTree*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_GLTree,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) TerrainPieceVA*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_TerrainPieceVA,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) HeightMap*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AntMap*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AntMap,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_HeightMap,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) AntMap*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AntMap,0);
   }
 }
 else vresult=Qnil;
}
%typemap(out) AntEntity*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AntEntity,0);
   }
 }
 else vresult=Qnil;
}
