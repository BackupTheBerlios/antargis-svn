%exception AGScreenWidget::AGScreenWidget {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGScreenWidget "general_markfunc"
%exception AGWindow::AGWindow {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGWindow "general_markfunc"
%exception AGCaption::AGCaption {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGCaption "general_markfunc"
%exception AGMessageObject::AGMessageObject {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGMessageObject "general_markfunc"
%exception AGTable::AGTable {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGTable "general_markfunc"
%exception AGText::AGText {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGText "general_markfunc"
%exception AGSound::AGSound {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGSound "general_markfunc"
%exception AGMenuItem::AGMenuItem {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGMenuItem "general_markfunc"
%exception AGMain::AGMain {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGMain "general_markfunc"
%exception AGVTiler::AGVTiler {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGVTiler "general_markfunc"
%exception AGCheckBox::AGCheckBox {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGCheckBox "general_markfunc"
%exception AGRadioGroup::AGRadioGroup {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGRadioGroup "general_markfunc"
%exception AGImage::AGImage {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGImage "general_markfunc"
%exception AGApplication::AGApplication {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGApplication "general_markfunc"
%exception AGDialog::AGDialog {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGDialog "general_markfunc"
%exception AGWidget::AGWidget {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGWidget "general_markfunc"
%exception AGGLWidget::AGGLWidget {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGGLWidget "general_markfunc"
%exception AGListener::AGListener {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGListener "general_markfunc"
%exception AGEdit::AGEdit {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGEdit "general_markfunc"
%exception AGLayout::AGLayout {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGLayout "general_markfunc"
%exception AGHTiler::AGHTiler {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGHTiler "general_markfunc"
%exception AGButton::AGButton {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGButton "general_markfunc"
%exception AGFrame::AGFrame {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGFrame "general_markfunc"
%exception AGSubMenu::AGSubMenu {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGSubMenu "general_markfunc"
%exception AGListBox::AGListBox {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGListBox "general_markfunc"
%exception AGColorButton::AGColorButton {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGColorButton "general_markfunc"
%exception AGMenu::AGMenu {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGMenu "general_markfunc"
%exception AGRubyObject::AGRubyObject {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGRubyObject "general_markfunc"
%exception AGRadio::AGRadio {
	$action
	result->mRUBY=self;
	result->mRubyObject=true;
}
%markfunc AGRadio "general_markfunc"
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
else if(dynamic_cast<AGSound*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSound,0);
else if(dynamic_cast<AGDialog*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGDialog,0);
else if(dynamic_cast<AGApplication*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGApplication,0);
else if(dynamic_cast<AGImage*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGImage,0);
else if(dynamic_cast<AGRadioGroup*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadioGroup,0);
else if(dynamic_cast<AGGLWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGGLWidget,0);
else if(dynamic_cast<AGEdit*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGEdit,0);
else if(dynamic_cast<AGListBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGFrame*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGFrame,0);
else if(dynamic_cast<AGColorButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<AGRadio*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<AGText*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGTable*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGTable,0);
else if(dynamic_cast<AGCheckBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGLayout*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<AGButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGHTiler*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGHTiler,0);
else if(dynamic_cast<AGVTiler*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGVTiler,0);
else if(dynamic_cast<AGWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWidget,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMessageObject,0);
   }
 }
 else vresult=Qnil;
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
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGTable,0);
   }
 }
 else vresult=Qnil;
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
%typemap(out) AGVTiler*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGHTiler*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGHTiler,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGVTiler,0);
   }
 }
 else vresult=Qnil;
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
%typemap(out) AGApplication*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGApplication,0);
   }
 }
 else vresult=Qnil;
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
else if(dynamic_cast<AGListBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGFrame*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGFrame,0);
else if(dynamic_cast<AGColorButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<AGRadio*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<AGText*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGTable*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGTable,0);
else if(dynamic_cast<AGCheckBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGLayout*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<AGButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGHTiler*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGHTiler,0);
else if(dynamic_cast<AGVTiler*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGVTiler,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGWidget,0);
   }
 }
 else vresult=Qnil;
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
else if(dynamic_cast<AGSound*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSound,0);
else if(dynamic_cast<AGDialog*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGDialog,0);
else if(dynamic_cast<AGApplication*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGApplication,0);
else if(dynamic_cast<AGImage*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGImage,0);
else if(dynamic_cast<AGRadioGroup*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadioGroup,0);
else if(dynamic_cast<AGGLWidget*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGGLWidget,0);
else if(dynamic_cast<AGEdit*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGEdit,0);
else if(dynamic_cast<AGListBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGListBox,0);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGFrame*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGFrame,0);
else if(dynamic_cast<AGColorButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGColorButton,0);
else if(dynamic_cast<AGMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenu,0);
else if(dynamic_cast<AGRadio*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGRadio,0);
else if(dynamic_cast<AGText*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGText,0);
else if(dynamic_cast<AGTable*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGTable,0);
else if(dynamic_cast<AGCheckBox*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGCheckBox,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
else if(dynamic_cast<AGLayout*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGLayout,0);
else if(dynamic_cast<AGButton*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGButton,0);
else if(dynamic_cast<AGHTiler*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGHTiler,0);
else if(dynamic_cast<AGVTiler*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGVTiler,0);
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
%typemap(out) AGHTiler*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
else if(dynamic_cast<AGSubMenu*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGSubMenu,0);
else if(dynamic_cast<AGMenuItem*>(result))
  vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGMenuItem,0);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGHTiler,0);
   }
 }
 else vresult=Qnil;
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
%typemap(out) AGFrame*{
 if($1)
 {
  if($1->mRubyObject)
    $result=$1->mRUBY;
  else
   {
     if(false);
   else
     vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AGFrame,0);
   }
 }
 else vresult=Qnil;
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
%typemap(directorout) AGFragProgram {
 AGFragProgram *b;
 Data_Get_Struct($input,AGFragProgram,b);
 $result=*b;
}
%typemap(directorout) AGFrame {
 AGFrame *b;
 Data_Get_Struct($input,AGFrame,b);
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
%typemap(directorout) AGHTiler {
 AGHTiler *b;
 Data_Get_Struct($input,AGHTiler,b);
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
%typemap(directorout) AGMouseEvent {
 AGMouseEvent *b;
 Data_Get_Struct($input,AGMouseEvent,b);
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
%typemap(directorout) AGRect23 {
 AGRect23 *b;
 Data_Get_Struct($input,AGRect23,b);
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
%typemap(directorout) AGShaderParameter {
 AGShaderParameter *b;
 Data_Get_Struct($input,AGShaderParameter,b);
 $result=*b;
}
%typemap(directorout) AGShaderProgram {
 AGShaderProgram *b;
 Data_Get_Struct($input,AGShaderProgram,b);
 $result=*b;
}
%typemap(directorout) AGSignal {
 AGSignal *b;
 Data_Get_Struct($input,AGSignal,b);
 $result=*b;
}
%typemap(directorout) AGSlot {
 AGSlot *b;
 Data_Get_Struct($input,AGSlot,b);
 $result=*b;
}
%typemap(directorout) AGSlot0 {
 AGSlot0 *b;
 Data_Get_Struct($input,AGSlot0,b);
 $result=*b;
}
%typemap(directorout) AGSlot2 {
 AGSlot2 *b;
 Data_Get_Struct($input,AGSlot2,b);
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
%typemap(directorout) AGVTiler {
 AGVTiler *b;
 Data_Get_Struct($input,AGVTiler,b);
 $result=*b;
}
%typemap(directorout) AGVector2 {
 AGVector2 *b;
 Data_Get_Struct($input,AGVector2,b);
 $result=*b;
}
%typemap(directorout) AGVector23 {
 AGVector23 *b;
 Data_Get_Struct($input,AGVector23,b);
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
%typemap(directorout) AGVertexProgram {
 AGVertexProgram *b;
 Data_Get_Struct($input,AGVertexProgram,b);
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
%typemap(directorout) Scene {
 Scene *b;
 Data_Get_Struct($input,Scene,b);
 $result=*b;
}
%typemap(directorout) Uint8 {
 $result=NUM2INT($input);
}
