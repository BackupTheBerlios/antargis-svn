%exception AGScroller::AGScroller {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGScroller "general_markfunc"
%exception AGScreenWidget::AGScreenWidget {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGScreenWidget "general_markfunc"
%exception AGWindow::AGWindow {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGWindow "general_markfunc"
%exception AGCaption::AGCaption {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGCaption "general_markfunc"
%exception AGMessageObject::AGMessageObject {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGMessageObject "general_markfunc"
%exception WaterPiece::WaterPiece {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc WaterPiece "general_markfunc"
%exception Scene::Scene {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc Scene "general_markfunc"
%exception AnimMeshData::AnimMeshData {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AnimMeshData "general_markfunc"
%exception AnimMesh::AnimMesh {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AnimMesh "general_markfunc"
%exception AGTable::AGTable {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGTable "general_markfunc"
%exception AGText::AGText {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGText "general_markfunc"
%exception NewDecal::NewDecal {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc NewDecal "general_markfunc"
%exception Mesh::Mesh {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc Mesh "general_markfunc"
%exception AGSound::AGSound {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGSound "general_markfunc"
%exception AGMenuItem::AGMenuItem {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGMenuItem "general_markfunc"
%exception AGMain::AGMain {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGMain "general_markfunc"
%exception AGCheckBox::AGCheckBox {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGCheckBox "general_markfunc"
%exception TerrainPiece::TerrainPiece {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc TerrainPiece "general_markfunc"
%exception AGRadioGroup::AGRadioGroup {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGRadioGroup "general_markfunc"
%exception AGImage::AGImage {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGImage "general_markfunc"
%exception AGApplication::AGApplication {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGApplication "general_markfunc"
%exception AGDialog::AGDialog {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGDialog "general_markfunc"
%exception AGWidget::AGWidget {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGWidget "general_markfunc"
%exception Smoke::Smoke {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc Smoke "general_markfunc"
%exception AGGLWidget::AGGLWidget {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGGLWidget "general_markfunc"
%exception AGListener::AGListener {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGListener "general_markfunc"
%exception MeshData::MeshData {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc MeshData "general_markfunc"
%exception AGEdit::AGEdit {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGEdit "general_markfunc"
%exception AGLayout::AGLayout {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGLayout "general_markfunc"
%exception AGButton::AGButton {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGButton "general_markfunc"
%exception SceneNode::SceneNode {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc SceneNode "general_markfunc"
%exception AGSubMenu::AGSubMenu {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGSubMenu "general_markfunc"
%exception AGListBox::AGListBox {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGListBox "general_markfunc"
%exception AGComboBox::AGComboBox {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGComboBox "general_markfunc"
%exception AGColorButton::AGColorButton {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGColorButton "general_markfunc"
%exception MiniMap::MiniMap {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc MiniMap "general_markfunc"
%exception AGMenu::AGMenu {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGMenu "general_markfunc"
%exception AGRubyObject::AGRubyObject {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGRubyObject "general_markfunc"
%exception AntMap::AntMap {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AntMap "general_markfunc"
%exception HeightMap::HeightMap {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc HeightMap "general_markfunc"
%exception AntEntity::AntEntity {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AntEntity "general_markfunc"
%exception GLApp::GLApp {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc GLApp "general_markfunc"
%exception AGRadio::AGRadio {
	$action
	result->mRUBY=self;
#ifdef GCDEBUG
     result->mObjName=typeid(*result).name();
     printf("%lx   %s\n",self,typeid(*result).name());
#endif
	result->mRubyObject=true;
}
%markfunc AGRadio "general_markfunc"
%typemap(out) AGScroller*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGScroller,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGScroller*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGScroller,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGScreenWidget*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGScreenWidget,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGScreenWidget*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGScreenWidget,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGWindow*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWindow,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGWindow*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGWindow,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGCaption*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCaption,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGCaption*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCaption,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGMessageObject*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGCaption*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCaption,0);
else if(dynamic_cast<AGWindow*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWindow,0);
else if(dynamic_cast<AGScreenWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGScreenWidget,0);
else if(dynamic_cast<AGScroller*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGScroller,0);
else if(dynamic_cast<AGSound*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSound,0);
else if(dynamic_cast<AGDialog*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGDialog,0);
else if(dynamic_cast<AGImage*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGImage,0);
else if(dynamic_cast<AGRadioGroup*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadioGroup,0);
else if(dynamic_cast<AGGLWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGGLWidget,0);
else if(dynamic_cast<AGEdit*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGEdit,0);
else if(dynamic_cast<AGComboBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGComboBox,0);
else if(dynamic_cast<AGListBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGColorButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<MiniMap*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_MiniMap,0);
else if(dynamic_cast<AGRadio*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<GLApp*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_GLApp,0);
else if(dynamic_cast<AntMap*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AntMap,0);
else if(dynamic_cast<AGText*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGCheckBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGApplication*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGApplication,0);
else if(dynamic_cast<AGLayout*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<HeightMap*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_HeightMap,0);
else if(dynamic_cast<AGButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGTable*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGTable,0);
else if(dynamic_cast<AGWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWidget,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMessageObject,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGMessageObject*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGCaption*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCaption,0);
else if(dynamic_cast<AGWindow*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGWindow,0);
else if(dynamic_cast<AGScreenWidget*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGScreenWidget,0);
else if(dynamic_cast<AGScroller*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGScroller,0);
else if(dynamic_cast<AGSound*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSound,0);
else if(dynamic_cast<AGDialog*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGDialog,0);
else if(dynamic_cast<AGImage*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGImage,0);
else if(dynamic_cast<AGRadioGroup*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadioGroup,0);
else if(dynamic_cast<AGGLWidget*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGGLWidget,0);
else if(dynamic_cast<AGEdit*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGEdit,0);
else if(dynamic_cast<AGComboBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGComboBox,0);
else if(dynamic_cast<AGListBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGColorButton*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<MiniMap*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_MiniMap,0);
else if(dynamic_cast<AGRadio*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<GLApp*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_GLApp,0);
else if(dynamic_cast<AntMap*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AntMap,0);
else if(dynamic_cast<AGText*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGCheckBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGApplication*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGApplication,0);
else if(dynamic_cast<AGLayout*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<HeightMap*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_HeightMap,0);
else if(dynamic_cast<AGButton*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGTable*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGTable,0);
else if(dynamic_cast<AGWidget*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGWidget,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMessageObject,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) WaterPiece*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_WaterPiece,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) Scene*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_Scene,0);
   }
 }
 else $input=Qnil;
}
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
%typemap(directorin) AnimMeshData*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AnimMeshData,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) AnimMesh*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AnimMesh,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGTable*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGWindow*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWindow,0);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGTable,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGTable*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGWindow*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGWindow,0);
else if(dynamic_cast<AGSubMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<AGMenuItem*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenuItem,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGTable,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGText*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGCaption*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCaption,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGText,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGText*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGCaption*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCaption,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGText,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) NewDecal*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_NewDecal,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) Mesh*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Mesh,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) Mesh*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_Mesh,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGSound*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSound,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGSound*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSound,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGMenuItem*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGMenuItem*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGSubMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSubMenu,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenuItem,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGMain*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMain,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGMain*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMain,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGCheckBox*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGRadio*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCheckBox,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGCheckBox*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGRadio*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadio,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCheckBox,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) TerrainPiece*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_TerrainPiece,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) TerrainPiece*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_TerrainPiece,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGRadioGroup*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadioGroup,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGRadioGroup*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadioGroup,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGImage*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGImage,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGImage*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGImage,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGApplication*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<GLApp*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_GLApp,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGApplication,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGApplication*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<GLApp*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_GLApp,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGApplication,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGDialog*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGDialog,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGDialog*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGDialog,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGWidget*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGCaption*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCaption,0);
else if(dynamic_cast<AGWindow*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWindow,0);
else if(dynamic_cast<AGScreenWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGScreenWidget,0);
else if(dynamic_cast<AGScroller*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGScroller,0);
else if(dynamic_cast<AGSound*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSound,0);
else if(dynamic_cast<AGDialog*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGDialog,0);
else if(dynamic_cast<AGImage*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGImage,0);
else if(dynamic_cast<AGRadioGroup*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadioGroup,0);
else if(dynamic_cast<AGGLWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGGLWidget,0);
else if(dynamic_cast<AGEdit*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGEdit,0);
else if(dynamic_cast<AGComboBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGComboBox,0);
else if(dynamic_cast<AGListBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGColorButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<MiniMap*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_MiniMap,0);
else if(dynamic_cast<AGRadio*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<AGText*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGCheckBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGLayout*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<AGButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGTable*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGTable,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWidget,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGWidget*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGCaption*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCaption,0);
else if(dynamic_cast<AGWindow*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGWindow,0);
else if(dynamic_cast<AGScreenWidget*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGScreenWidget,0);
else if(dynamic_cast<AGScroller*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGScroller,0);
else if(dynamic_cast<AGSound*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSound,0);
else if(dynamic_cast<AGDialog*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGDialog,0);
else if(dynamic_cast<AGImage*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGImage,0);
else if(dynamic_cast<AGRadioGroup*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadioGroup,0);
else if(dynamic_cast<AGGLWidget*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGGLWidget,0);
else if(dynamic_cast<AGEdit*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGEdit,0);
else if(dynamic_cast<AGComboBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGComboBox,0);
else if(dynamic_cast<AGListBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGColorButton*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<MiniMap*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_MiniMap,0);
else if(dynamic_cast<AGRadio*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<AGText*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGCheckBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGLayout*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<AGButton*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGTable*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGTable,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGWidget,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) Smoke*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_Smoke,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGGLWidget*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGGLWidget,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGGLWidget*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGGLWidget,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGListener*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGCaption*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCaption,0);
else if(dynamic_cast<AGWindow*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWindow,0);
else if(dynamic_cast<AGScreenWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGScreenWidget,0);
else if(dynamic_cast<AGScroller*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGScroller,0);
else if(dynamic_cast<AGSound*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSound,0);
else if(dynamic_cast<AGDialog*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGDialog,0);
else if(dynamic_cast<AGImage*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGImage,0);
else if(dynamic_cast<AGRadioGroup*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadioGroup,0);
else if(dynamic_cast<AGGLWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGGLWidget,0);
else if(dynamic_cast<AGEdit*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGEdit,0);
else if(dynamic_cast<AGComboBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGComboBox,0);
else if(dynamic_cast<AGListBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGColorButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<MiniMap*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_MiniMap,0);
else if(dynamic_cast<AGRadio*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<GLApp*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_GLApp,0);
else if(dynamic_cast<AntMap*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AntMap,0);
else if(dynamic_cast<AGText*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGCheckBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGApplication*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGApplication,0);
else if(dynamic_cast<AGLayout*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<HeightMap*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_HeightMap,0);
else if(dynamic_cast<AGButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGTable*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGTable,0);
else if(dynamic_cast<AGWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWidget,0);
else if(dynamic_cast<AGMessageObject*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMessageObject,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGListener,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGListener*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGCaption*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCaption,0);
else if(dynamic_cast<AGWindow*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGWindow,0);
else if(dynamic_cast<AGScreenWidget*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGScreenWidget,0);
else if(dynamic_cast<AGScroller*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGScroller,0);
else if(dynamic_cast<AGSound*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSound,0);
else if(dynamic_cast<AGDialog*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGDialog,0);
else if(dynamic_cast<AGImage*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGImage,0);
else if(dynamic_cast<AGRadioGroup*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadioGroup,0);
else if(dynamic_cast<AGGLWidget*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGGLWidget,0);
else if(dynamic_cast<AGEdit*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGEdit,0);
else if(dynamic_cast<AGComboBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGComboBox,0);
else if(dynamic_cast<AGListBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGColorButton*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<MiniMap*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_MiniMap,0);
else if(dynamic_cast<AGRadio*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<GLApp*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_GLApp,0);
else if(dynamic_cast<AntMap*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AntMap,0);
else if(dynamic_cast<AGText*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGCheckBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGApplication*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGApplication,0);
else if(dynamic_cast<AGLayout*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<HeightMap*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_HeightMap,0);
else if(dynamic_cast<AGButton*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGTable*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGTable,0);
else if(dynamic_cast<AGWidget*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGWidget,0);
else if(dynamic_cast<AGMessageObject*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMessageObject,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGListener,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) MeshData*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_MeshData,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGEdit*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGEdit,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGEdit*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGEdit,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGLayout*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGDialog*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGDialog,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGLayout,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGLayout*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGDialog*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGDialog,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGLayout,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGButton*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGRadio*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<AGCheckBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCheckBox,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGButton,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGButton*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGRadio*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<AGCheckBox*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGCheckBox,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGButton,0);
   }
 }
 else $input=Qnil;
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
else if(dynamic_cast<Mesh*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Mesh,0);
else if(dynamic_cast<NewDecal*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_NewDecal,0);
else if(dynamic_cast<TerrainPiece*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_TerrainPiece,0);
else if(dynamic_cast<Smoke*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Smoke,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_SceneNode,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) SceneNode*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AnimMesh*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AnimMesh,0);
else if(dynamic_cast<WaterPiece*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_WaterPiece,0);
else if(dynamic_cast<Mesh*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_Mesh,0);
else if(dynamic_cast<NewDecal*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_NewDecal,0);
else if(dynamic_cast<TerrainPiece*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_TerrainPiece,0);
else if(dynamic_cast<Smoke*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_Smoke,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_SceneNode,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGSubMenu*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGSubMenu*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGSubMenu,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGListBox*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGListBox,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGListBox*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGListBox,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGComboBox*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGComboBox,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGComboBox*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGComboBox,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGColorButton*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGColorButton,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGColorButton*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGColorButton,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) MiniMap*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_MiniMap,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) MiniMap*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_MiniMap,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGMenu*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGMenu*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGMenu,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) AntMap*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AntMap,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) HeightMap*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AntMap*>($1))
  $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AntMap,0);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_HeightMap,0);
   }
 }
 else $input=Qnil;
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
%typemap(directorin) AntEntity*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AntEntity,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) GLApp*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_GLApp,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) GLApp*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_GLApp,0);
   }
 }
 else $input=Qnil;
}
%typemap(out) AGRadio*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
   }
 }
 else vresult=Qnil;
}
%typemap(directorin) AGRadio*{
 if($1)
 {
  if($1->mRubyObject)
    $input=$1->mRUBY;
  else
   {
     if(false);
   else
     $input = SWIG_NewPointerObj((void *)$1, SWIGTYPE_p_AGRadio,0);
   }
 }
 else $input=Qnil;
}
%typemap(directorout) AGApplication {
 AGApplication *b;
 Data_Get_Struct($input,AGApplication,b);
 $result=*b;
}
%typemap(directorout) AGBackground {
 AGBackground *b;
 Data_Get_Struct($input,AGBackground,b);
 $result=*b;
}
%typemap(directorout) AGBorder {
 AGBorder *b;
 Data_Get_Struct($input,AGBorder,b);
 $result=*b;
}
%typemap(directorout) AGBox3 {
 AGBox3 *b;
 Data_Get_Struct($input,AGBox3,b);
 $result=*b;
}
%typemap(directorout) AGButton {
 AGButton *b;
 Data_Get_Struct($input,AGButton,b);
 $result=*b;
}
%typemap(directorout) AGCPPListener {
 AGCPPListener *b;
 Data_Get_Struct($input,AGCPPListener,b);
 $result=*b;
}
%typemap(directorout) AGCaption {
 AGCaption *b;
 Data_Get_Struct($input,AGCaption,b);
 $result=*b;
}
%typemap(directorout) AGCheckBox {
 AGCheckBox *b;
 Data_Get_Struct($input,AGCheckBox,b);
 $result=*b;
}
%typemap(directorout) AGCircle2 {
 AGCircle2 *b;
 Data_Get_Struct($input,AGCircle2,b);
 $result=*b;
}
%typemap(directorout) AGCollisionData {
 AGCollisionData *b;
 Data_Get_Struct($input,AGCollisionData,b);
 $result=*b;
}
%typemap(directorout) AGColor {
 AGColor *b;
 Data_Get_Struct($input,AGColor,b);
 $result=*b;
}
%typemap(directorout) AGColorButton {
 AGColorButton *b;
 Data_Get_Struct($input,AGColorButton,b);
 $result=*b;
}
%typemap(directorout) AGComboBox {
 AGComboBox *b;
 Data_Get_Struct($input,AGComboBox,b);
 $result=*b;
}
%typemap(directorout) AGConfig {
 AGConfig *b;
 Data_Get_Struct($input,AGConfig,b);
 $result=*b;
}
%typemap(directorout) AGDecryptor {
 AGDecryptor *b;
 Data_Get_Struct($input,AGDecryptor,b);
 $result=*b;
}
%typemap(directorout) AGDialog {
 AGDialog *b;
 Data_Get_Struct($input,AGDialog,b);
 $result=*b;
}
%typemap(directorout) AGEdit {
 AGEdit *b;
 Data_Get_Struct($input,AGEdit,b);
 $result=*b;
}
%typemap(directorout) AGEditLine {
 AGEditLine *b;
 Data_Get_Struct($input,AGEditLine,b);
 $result=*b;
}
%typemap(directorout) AGEvent {
 AGEvent *b;
 Data_Get_Struct($input,AGEvent,b);
 $result=*b;
}
%typemap(directorout) AGFBO {
 AGFBO *b;
 Data_Get_Struct($input,AGFBO,b);
 $result=*b;
}
%typemap(directorout) AGFont {
 AGFont *b;
 Data_Get_Struct($input,AGFont,b);
 $result=*b;
}
%typemap(directorout) AGFontEngine {
 AGFontEngine *b;
 Data_Get_Struct($input,AGFontEngine,b);
 $result=*b;
}
%typemap(directorout) AGGLTexture {
 AGGLTexture *b;
 Data_Get_Struct($input,AGGLTexture,b);
 $result=*b;
}
%typemap(directorout) AGGLWidget {
 AGGLWidget *b;
 Data_Get_Struct($input,AGGLWidget,b);
 $result=*b;
}
%typemap(directorout) AGImage {
 AGImage *b;
 Data_Get_Struct($input,AGImage,b);
 $result=*b;
}
%typemap(directorout) AGInternalSurface {
 AGInternalSurface *b;
 Data_Get_Struct($input,AGInternalSurface,b);
 $result=*b;
}
%typemap(directorout) AGLayout {
 AGLayout *b;
 Data_Get_Struct($input,AGLayout,b);
 $result=*b;
}
%typemap(directorout) AGLayoutCreator {
 AGLayoutCreator *b;
 Data_Get_Struct($input,AGLayoutCreator,b);
 $result=*b;
}
%typemap(directorout) AGLayoutFactory {
 AGLayoutFactory *b;
 Data_Get_Struct($input,AGLayoutFactory,b);
 $result=*b;
}
%typemap(directorout) AGLine2 {
 AGLine2 *b;
 Data_Get_Struct($input,AGLine2,b);
 $result=*b;
}
%typemap(directorout) AGLine3 {
 AGLine3 *b;
 Data_Get_Struct($input,AGLine3,b);
 $result=*b;
}
%typemap(directorout) AGList {
 AGList *b;
 Data_Get_Struct($input,AGList,b);
 $result=*b;
}
%typemap(directorout) AGListBox {
 AGListBox *b;
 Data_Get_Struct($input,AGListBox,b);
 $result=*b;
}
%typemap(directorout) AGListIterator {
 AGListIterator *b;
 Data_Get_Struct($input,AGListIterator,b);
 $result=*b;
}
%typemap(directorout) AGListener {
 AGListener *b;
 Data_Get_Struct($input,AGListener,b);
 $result=*b;
}
%typemap(directorout) AGLocalizer {
 AGLocalizer *b;
 Data_Get_Struct($input,AGLocalizer,b);
 $result=*b;
}
%typemap(directorout) AGMain {
 AGMain *b;
 Data_Get_Struct($input,AGMain,b);
 $result=*b;
}
%typemap(directorout) AGMatrix3 {
 AGMatrix3 *b;
 Data_Get_Struct($input,AGMatrix3,b);
 $result=*b;
}
%typemap(directorout) AGMatrix4 {
 AGMatrix4 *b;
 Data_Get_Struct($input,AGMatrix4,b);
 $result=*b;
}
%typemap(directorout) AGMenu {
 AGMenu *b;
 Data_Get_Struct($input,AGMenu,b);
 $result=*b;
}
%typemap(directorout) AGMenuItem {
 AGMenuItem *b;
 Data_Get_Struct($input,AGMenuItem,b);
 $result=*b;
}
%typemap(directorout) AGMessageObject {
 AGMessageObject *b;
 Data_Get_Struct($input,AGMessageObject,b);
 $result=*b;
}
%typemap(directorout) AGMutex {
 AGMutex *b;
 Data_Get_Struct($input,AGMutex,b);
 $result=*b;
}
%typemap(directorout) AGPaintTarget {
 AGPaintTarget *b;
 Data_Get_Struct($input,AGPaintTarget,b);
 $result=*b;
}
%typemap(directorout) AGPainter {
 AGPainter *b;
 Data_Get_Struct($input,AGPainter,b);
 $result=*b;
}
%typemap(directorout) AGRadio {
 AGRadio *b;
 Data_Get_Struct($input,AGRadio,b);
 $result=*b;
}
%typemap(directorout) AGRadioGroup {
 AGRadioGroup *b;
 Data_Get_Struct($input,AGRadioGroup,b);
 $result=*b;
}
%typemap(directorout) AGRect2 {
 AGRect2 *b;
 Data_Get_Struct($input,AGRect2,b);
 $result=*b;
}
%typemap(directorout) AGRect3 {
 AGRect3 *b;
 Data_Get_Struct($input,AGRect3,b);
 $result=*b;
}
%typemap(directorout) AGRenderContext {
 AGRenderContext *b;
 Data_Get_Struct($input,AGRenderContext,b);
 $result=*b;
}
%typemap(directorout) AGRubyObject {
 AGRubyObject *b;
 Data_Get_Struct($input,AGRubyObject,b);
 $result=*b;
}
%typemap(directorout) AGScreen {
 AGScreen *b;
 Data_Get_Struct($input,AGScreen,b);
 $result=*b;
}
%typemap(directorout) AGScreenWidget {
 AGScreenWidget *b;
 Data_Get_Struct($input,AGScreenWidget,b);
 $result=*b;
}
%typemap(directorout) AGScroller {
 AGScroller *b;
 Data_Get_Struct($input,AGScroller,b);
 $result=*b;
}
%typemap(directorout) AGSignal {
 AGSignal *b;
 Data_Get_Struct($input,AGSignal,b);
 $result=*b;
}
%typemap(directorout) AGSlot0 {
 AGSlot0 *b;
 Data_Get_Struct($input,AGSlot0,b);
 $result=*b;
}
%typemap(directorout) AGSound {
 AGSound *b;
 Data_Get_Struct($input,AGSound,b);
 $result=*b;
}
%typemap(directorout) AGSubMenu {
 AGSubMenu *b;
 Data_Get_Struct($input,AGSubMenu,b);
 $result=*b;
}
%typemap(directorout) AGSurface {
 AGSurface *b;
 Data_Get_Struct($input,AGSurface,b);
 $result=*b;
}
%typemap(directorout) AGSurfaceManager {
 AGSurfaceManager *b;
 Data_Get_Struct($input,AGSurfaceManager,b);
 $result=*b;
}
%typemap(directorout) AGTable {
 AGTable *b;
 Data_Get_Struct($input,AGTable,b);
 $result=*b;
}
%typemap(directorout) AGText {
 AGText *b;
 Data_Get_Struct($input,AGText,b);
 $result=*b;
}
%typemap(directorout) AGTexture {
 AGTexture *b;
 Data_Get_Struct($input,AGTexture,b);
 $result=*b;
}
%typemap(directorout) AGTextureCache {
 AGTextureCache *b;
 Data_Get_Struct($input,AGTextureCache,b);
 $result=*b;
}
%typemap(directorout) AGTheme {
 AGTheme *b;
 Data_Get_Struct($input,AGTheme,b);
 $result=*b;
}
%typemap(directorout) AGTooltip {
 AGTooltip *b;
 Data_Get_Struct($input,AGTooltip,b);
 $result=*b;
}
%typemap(directorout) AGTriangle2 {
 AGTriangle2 *b;
 Data_Get_Struct($input,AGTriangle2,b);
 $result=*b;
}
%typemap(directorout) AGTriangle3 {
 AGTriangle3 *b;
 Data_Get_Struct($input,AGTriangle3,b);
 $result=*b;
}
%typemap(directorout) AGVector2 {
 AGVector2 *b;
 Data_Get_Struct($input,AGVector2,b);
 $result=*b;
}
%typemap(directorout) AGVector3 {
 AGVector3 *b;
 Data_Get_Struct($input,AGVector3,b);
 $result=*b;
}
%typemap(directorout) AGVector4 {
 AGVector4 *b;
 Data_Get_Struct($input,AGVector4,b);
 $result=*b;
}
%typemap(directorout) AGWidget {
 AGWidget *b;
 Data_Get_Struct($input,AGWidget,b);
 $result=*b;
}
%typemap(directorout) AGWindow {
 AGWindow *b;
 Data_Get_Struct($input,AGWindow,b);
 $result=*b;
}
%typemap(directorout) AnimMesh {
 AnimMesh *b;
 Data_Get_Struct($input,AnimMesh,b);
 $result=*b;
}
%typemap(directorout) AnimMeshData {
 AnimMeshData *b;
 Data_Get_Struct($input,AnimMeshData,b);
 $result=*b;
}
%typemap(directorout) AntCamera {
 AntCamera *b;
 Data_Get_Struct($input,AntCamera,b);
 $result=*b;
}
%typemap(directorout) AntEntity {
 AntEntity *b;
 Data_Get_Struct($input,AntEntity,b);
 $result=*b;
}
%typemap(directorout) AntEntityPtr {
 AntEntityPtr *b;
 Data_Get_Struct($input,AntEntityPtr,b);
 $result=*b;
}
%typemap(directorout) AntFragProgram {
 AntFragProgram *b;
 Data_Get_Struct($input,AntFragProgram,b);
 $result=*b;
}
%typemap(directorout) AntFrustum {
 AntFrustum *b;
 Data_Get_Struct($input,AntFrustum,b);
 $result=*b;
}
%typemap(directorout) AntMap {
 AntMap *b;
 Data_Get_Struct($input,AntMap,b);
 $result=*b;
}
%typemap(directorout) AntPlane {
 AntPlane *b;
 Data_Get_Struct($input,AntPlane,b);
 $result=*b;
}
%typemap(directorout) AntProjection {
 AntProjection *b;
 Data_Get_Struct($input,AntProjection,b);
 $result=*b;
}
%typemap(directorout) AntShaderParameter {
 AntShaderParameter *b;
 Data_Get_Struct($input,AntShaderParameter,b);
 $result=*b;
}
%typemap(directorout) AntShaderProgram {
 AntShaderProgram *b;
 Data_Get_Struct($input,AntShaderProgram,b);
 $result=*b;
}
%typemap(directorout) AntShadowShader {
 AntShadowShader *b;
 Data_Get_Struct($input,AntShadowShader,b);
 $result=*b;
}
%typemap(directorout) AntVertexProgram {
 AntVertexProgram *b;
 Data_Get_Struct($input,AntVertexProgram,b);
 $result=*b;
}
%typemap(directorout) AntWaterShader {
 AntWaterShader *b;
 Data_Get_Struct($input,AntWaterShader,b);
 $result=*b;
}
%typemap(directorout) BinaryFileIn {
 BinaryFileIn *b;
 Data_Get_Struct($input,BinaryFileIn,b);
 $result=*b;
}
%typemap(directorout) BinaryFileOut {
 BinaryFileOut *b;
 Data_Get_Struct($input,BinaryFileOut,b);
 $result=*b;
}
%typemap(directorout) BinaryIn {
 BinaryIn *b;
 Data_Get_Struct($input,BinaryIn,b);
 $result=*b;
}
%typemap(directorout) BinaryOut {
 BinaryOut *b;
 Data_Get_Struct($input,BinaryOut,b);
 $result=*b;
}
%typemap(directorout) BinaryStringIn {
 BinaryStringIn *b;
 Data_Get_Struct($input,BinaryStringIn,b);
 $result=*b;
}
%typemap(directorout) BinaryStringOut {
 BinaryStringOut *b;
 Data_Get_Struct($input,BinaryStringOut,b);
 $result=*b;
}
%typemap(directorout) Document {
 Document *b;
 Data_Get_Struct($input,Document,b);
 $result=*b;
}
%typemap(directorout) DomParser {
 DomParser *b;
 Data_Get_Struct($input,DomParser,b);
 $result=*b;
}
%typemap(directorout) GLApp {
 GLApp *b;
 Data_Get_Struct($input,GLApp,b);
 $result=*b;
}
%typemap(directorout) HeightMap {
 HeightMap *b;
 Data_Get_Struct($input,HeightMap,b);
 $result=*b;
}
%typemap(directorout) Job {
 Job *b;
 Data_Get_Struct($input,Job,b);
 $result=*b;
}
%typemap(directorout) Mesh {
 Mesh *b;
 Data_Get_Struct($input,Mesh,b);
 $result=*b;
}
%typemap(directorout) MeshData {
 MeshData *b;
 Data_Get_Struct($input,MeshData,b);
 $result=*b;
}
%typemap(directorout) MeshOptimizer {
 MeshOptimizer *b;
 Data_Get_Struct($input,MeshOptimizer,b);
 $result=*b;
}
%typemap(directorout) MiniMap {
 MiniMap *b;
 Data_Get_Struct($input,MiniMap,b);
 $result=*b;
}
%typemap(directorout) NewDecal {
 NewDecal *b;
 Data_Get_Struct($input,NewDecal,b);
 $result=*b;
}
%typemap(directorout) Node {
 Node *b;
 Data_Get_Struct($input,Node,b);
 $result=*b;
}
%typemap(directorout) Parser {
 Parser *b;
 Data_Get_Struct($input,Parser,b);
 $result=*b;
}
%typemap(directorout) QuadTree {
 QuadTree *b;
 Data_Get_Struct($input,QuadTree,b);
 $result=*b;
}
%typemap(directorout) Renderer {
 Renderer *b;
 Data_Get_Struct($input,Renderer,b);
 $result=*b;
}
%typemap(directorout) Resource {
 Resource *b;
 Data_Get_Struct($input,Resource,b);
 $result=*b;
}
%typemap(directorout) Scene {
 Scene *b;
 Data_Get_Struct($input,Scene,b);
 $result=*b;
}
%typemap(directorout) SceneNode {
 SceneNode *b;
 Data_Get_Struct($input,SceneNode,b);
 $result=*b;
}
%typemap(directorout) SerialException {
 SerialException *b;
 Data_Get_Struct($input,SerialException,b);
 $result=*b;
}
%typemap(directorout) Smoke {
 Smoke *b;
 Data_Get_Struct($input,Smoke,b);
 $result=*b;
}
%typemap(directorout) Terrain {
 Terrain *b;
 Data_Get_Struct($input,Terrain,b);
 $result=*b;
}
%typemap(directorout) TerrainPiece {
 TerrainPiece *b;
 Data_Get_Struct($input,TerrainPiece,b);
 $result=*b;
}
%typemap(directorout) VertexArray {
 VertexArray *b;
 Data_Get_Struct($input,VertexArray,b);
 $result=*b;
}
%typemap(directorout) VertexArrayShader {
 VertexArrayShader *b;
 Data_Get_Struct($input,VertexArrayShader,b);
 $result=*b;
}
%typemap(directorout) WaterPiece {
 WaterPiece *b;
 Data_Get_Struct($input,WaterPiece,b);
 $result=*b;
}
%typemap(directorout) Uint8 {
 $result=NUM2INT($input);
}
